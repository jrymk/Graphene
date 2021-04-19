#include "imguipanelmanager.h"
//#include <stdio.h> // fprintf

#ifndef NO_IMGUITABWINDOW
#include "../imguitabwindow/imguitabwindow.h"
#endif //NO_IMGUITABWINDOW


bool gImGuiDockpanelManagerExtendedStyle = true;
float gImGuiDockPanelManagerActiveResizeSize = 8.f;
bool gImGuiDockPanelManagerAddExtraTitleBarResizing = false;
bool gImGuiDockPanelManagerAlwaysDrawExternalBorders = true;

// Static Helper Methods:
namespace ImGui {

static ImVec4 ColorDockButton(0.75f, 0.75f, 0.30f, 0.50f);
static ImVec4 ColorDockButtonHovered(0.85f, 0.85f, 0.5f, 0.60f);
static ImVec4 ColorDockButtonActive(0.70f, 0.70f, 0.60f, 1.00f);

static ImVec4 ColorDockButtonLines(0.80f, 0.90f, 0.70f, 1.00f);
static ImVec4 ColorDockButtonLinesHovered(0.85f, 1.f, 0.65f, 1.00f);

static ImVec4 ColorDockButtonIconBg(0.75f, 0.75f, 0.75f, 0.70f);
static ImVec4 ColorDockButtonIconBgHovered(0.85f, 0.85f, 0.85f, 0.85f);
static ImVec4 ColorDockButtonIconBgActive(0.70f, 0.70f, 0.70f, 1.00f);

static ImVec4 ColorDockButtonIcon(0.80f, 0.90f, 0.70f, 1.00f);
static ImVec4 ColorDockButtonIconHovered(0.85f, 1.f, 0.65f, 1.00f);


// Upper-right button to close a window.
static bool DockWindowButton(bool* p_undocked,bool *p_open=NULL)
{
    ImGuiWindow* window = GetCurrentWindow();

    const ImGuiID id = window->GetID("#DOCK");
    const float size = window->TitleBarHeight() - 4.0f;
    const ImRect bb(window->Rect().GetTR() + ImVec2(-3.0f-size,2.0f) + (p_open ? ImVec2(-3.0f-size,0) : ImVec2(0,0)),
                       window->Rect().GetTR() + ImVec2(-3.0f,2.0f+size) + (p_open ? ImVec2(-3.0f-size,0) : ImVec2(0,0))
                       );

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);//, true);

    if (p_undocked) {
#       ifdef IMGUITABWINDOW_H_
        if (gImGuiDockpanelManagerExtendedStyle && ImGui::TabWindow::DockPanelIconTextureID)    {
            // Render
            ImU32 col = GetColorU32((held && hovered) ? ColorDockButtonIconBgActive : hovered ? ColorDockButtonIconBgHovered : ColorDockButtonIconBg);
            window->DrawList->AddRectFilled(bb.Min, bb.Max, col,2.f);

            col = !hovered ? GetColorU32(ColorDockButtonIcon) : GetColorU32(ColorDockButtonIconHovered);
            if (*p_undocked)    {
               window->DrawList->AddImage(ImGui::TabWindow::DockPanelIconTextureID,bb.Min,bb.Max,ImVec2(0.0f,0.75f),ImVec2(0.25f,1.f), col);
            }
            else    {
               window->DrawList->AddImage(ImGui::TabWindow::DockPanelIconTextureID,bb.Min,bb.Max,ImVec2(0.25f,0.75f),ImVec2(0.5f,1.f), col);
            }
        }
        else
#       endif //IMGUITABWINDOW_H_
        {
            // Render
            ImU32 col = GetColorU32((held && hovered) ? ColorDockButtonActive : hovered ? ColorDockButtonHovered : ColorDockButton);
            window->DrawList->AddRectFilled(bb.Min, bb.Max, col, 0);

            col = !hovered ? GetColorU32(ColorDockButtonLines) : GetColorU32(ColorDockButtonLinesHovered);
            ImRect bb2 = bb;
            const ImVec2  sz = bb.GetSize();
            if (*p_undocked)    {
                bb2.Expand(ImVec2(-.2f*sz.x,-.7*sz.y));
                window->DrawList->AddRect(bb2.Min,bb2.Max, col, 0);
            }
            else    {
                bb2.Expand(ImVec2(-.7f*sz.x,-.2*sz.y));
                window->DrawList->AddRect(bb2.Min,bb2.Max, col, 0);
            }
        }
    }
    else {
        // Render
        ImU32 col = GetColorU32((held && hovered) ? ColorDockButtonActive : hovered ? ColorDockButtonHovered : ColorDockButton);
        window->DrawList->AddRectFilled(bb.Min, bb.Max, col, 0);
    }

    if (p_undocked != NULL && pressed)
        *p_undocked = !*p_undocked;

    return pressed;
}
static bool DockWindowBegin(const char* name, bool* p_opened,bool* p_undocked, const ImVec2& size, float bg_alpha, ImGuiWindowFlags flags,bool* pDraggingStarted=NULL,ImGui::PanelManager::WindowData* wd=NULL)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    IM_ASSERT(name != NULL && name[0] != '\0');                        // Window name required
    IM_ASSERT(g.Initialized);                       // Forgot to call ImGui::NewFrame()
    IM_ASSERT(g.WithinFrameScope);                  // Forgot to call ImGui::NewFrame()
    IM_ASSERT(g.FrameCountEnded != g.FrameCount);   // Called ImGui::Render() or ImGui::EndFrame() and haven't called ImGui::NewFrame() again yet

    // Find or create
    ImGuiWindow* window = FindWindowByName(name);
    const bool window_just_created = (window == NULL);
    if (window_just_created)
    {
        //ImVec2 size_on_first_use = (g.NextWindowData.SizeCond != 0) ? g.NextWindowData.SizeVal : ImVec2(0.0f, 0.0f); // Any condition flag will do since we are creating a new window here.
        window = CreateNewWindow(name, /*size_on_first_use,*/ flags);
    }

    // Automatically disable manual moving/resizing when NoInputs is set
    if (flags & ImGuiWindowFlags_NoInputs)
        flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    if (flags & ImGuiWindowFlags_NavFlattened)
        IM_ASSERT(flags & ImGuiWindowFlags_ChildWindow);

    const int current_frame = g.FrameCount;
    const bool first_begin_of_the_frame = (window->LastFrameActive != current_frame);
    if (first_begin_of_the_frame)
        window->Flags = (ImGuiWindowFlags)flags;
    else
        flags = window->Flags;

    // Parent window is latched only on the first call to Begin() of the frame, so further append-calls can be done from a different window stack
    ImGuiWindow* parent_window_in_stack = g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back();
    ImGuiWindow* parent_window = first_begin_of_the_frame ? ((flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)) ? parent_window_in_stack : NULL) : window->ParentWindow;
    IM_ASSERT(parent_window != NULL || !(flags & ImGuiWindowFlags_ChildWindow));
    window->HasCloseButton = (p_opened != NULL);

    // Update the Appearing flag
    bool window_just_activated_by_user = (window->LastFrameActive < current_frame - 1);   // Not using !WasActive because the implicit "Debug" window would always toggle off->on
    //const bool window_just_appearing_after_hidden_for_resize = (window->HiddenFramesCannotSkipItems > 0);
    if (flags & ImGuiWindowFlags_Popup)
    {
        ImGuiPopupData& popup_ref = g.OpenPopupStack[g.BeginPopupStack.Size];
        window_just_activated_by_user |= (window->PopupId != popup_ref.PopupId); // We recycle popups so treat window as activated if popup id changed
        window_just_activated_by_user |= (window != popup_ref.Window);
    }
    //window->Appearing = (window_just_activated_by_user || window_just_appearing_after_hidden_for_resize);
    window->Appearing = window_just_activated_by_user;
    if (window->Appearing)
        SetWindowConditionAllowFlags(window, ImGuiCond_Appearing, true);

    // We allow window memory to be compacted so recreate the base stack when needed.
        if (window->IDStack.Size == 0)
            window->IDStack.push_back(window->ID);

    // Add to stack
    // We intentionally set g.CurrentWindow to NULL to prevent usage until when the viewport is set, then will call SetCurrentWindow()
    g.CurrentWindowStack.push_back(window);
    g.CurrentWindow = window;
    window->DC.StackSizesOnBegin.SetToCurrentState();
    g.CurrentWindow = NULL;

    if (flags & ImGuiWindowFlags_Popup)
    {
        ImGuiPopupData& popup_ref = g.OpenPopupStack[g.BeginPopupStack.Size];
        popup_ref.Window = window;
        g.BeginPopupStack.push_back(popup_ref);
        window->PopupId = popup_ref.PopupId;
    }

    // Must we remove this?
    if (window->HiddenFramesCannotSkipItems>0 && !(flags & ImGuiWindowFlags_ChildWindow))
        window->NavLastIds[0] = 0;

    // Update ->RootWindow and others pointers (before any possible call to FocusWindow)
    if (first_begin_of_the_frame)
        UpdateWindowParentAndRootLinks(window, flags, parent_window);

    // Process SetNextWindow***() calls
    bool window_pos_set_by_api = false;
    bool window_size_x_set_by_api = false, window_size_y_set_by_api = false;
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasPos)
    {
        window_pos_set_by_api = (window->SetWindowPosAllowFlags & g.NextWindowData.PosCond) != 0;
        if (window_pos_set_by_api && ImLengthSqr(g.NextWindowData.PosPivotVal) > 0.00001f)
        {
            // May be processed on the next frame if this is our first frame and we are measuring size
            // FIXME: Look into removing the branch so everything can go through this same code path for consistency.
            window->SetWindowPosVal = g.NextWindowData.PosVal;
            window->SetWindowPosPivot = g.NextWindowData.PosPivotVal;
            window->SetWindowPosAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
        }
        else
        {
            SetWindowPos(window, g.NextWindowData.PosVal, g.NextWindowData.PosCond);
        }
    }
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize)
    {
        window_size_x_set_by_api = (window->SetWindowSizeAllowFlags & g.NextWindowData.SizeCond) != 0 && (g.NextWindowData.SizeVal.x > 0.0f);
        window_size_y_set_by_api = (window->SetWindowSizeAllowFlags & g.NextWindowData.SizeCond) != 0 && (g.NextWindowData.SizeVal.y > 0.0f);
        SetWindowSize(window, g.NextWindowData.SizeVal, g.NextWindowData.SizeCond);
    }
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasContentSize)
        window->ContentSizeExplicit = g.NextWindowData.ContentSizeVal;
    else if (first_begin_of_the_frame)
        window->ContentSizeExplicit = ImVec2(0.0f, 0.0f);
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasCollapsed)
        SetWindowCollapsed(window, g.NextWindowData.CollapsedVal, g.NextWindowData.CollapsedCond);
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasFocus)
        FocusWindow(window);
    if (window->Appearing)
        SetWindowConditionAllowFlags(window, ImGuiCond_Appearing, false);

    // Default alpha
    //if (bg_alpha < 0.0f) bg_alpha = 0.7f;    //1.0f; //It was 0.7f (e.g. style.WindowFillAlphaDefault);
    const ImGuiCol bg_color_idx = GetWindowBgColorIdxFromFlags(flags);
    const ImVec4 bg_color_backup = g.Style.Colors[bg_color_idx];
    if (bg_alpha >= 0.0f) g.Style.Colors[bg_color_idx].w = bg_alpha;

    // When reusing window again multiple times a frame, just append content (don't need to setup again)
    if (first_begin_of_the_frame)
    {
        // Initialize
        const bool window_is_child_tooltip = (flags & ImGuiWindowFlags_ChildWindow) && (flags & ImGuiWindowFlags_Tooltip); // FIXME-WIP: Undocumented behavior of Child+Tooltip for pinned tooltip (#1345)

        window->Active = true;
        window->BeginOrderWithinParent = 0;
        window->BeginOrderWithinContext = g.WindowsActiveCount++;
        window->BeginCount = 0;
        window->ClipRect = ImVec4(-FLT_MAX,-FLT_MAX,+FLT_MAX,+FLT_MAX);
        window->LastFrameActive = current_frame;
        window->LastTimeActive = (float) g.Time;
        window->IDStack.resize(1);
        window->DrawList->_ResetForNewFrame();
        window->DC.CurrentTableIdx = -1;


        // Restore buffer capacity when woken from a compacted state, to avoid
        if (window->MemoryCompacted)
            GcAwakeTransientWindowBuffers(window);

        // Update stored window name when it changes (which can _only_ happen with the "###" operator, so the ID would stay unchanged).
        // The title bar always display the 'name' parameter, so we only update the string storage if it needs to be visible to the end-user elsewhere.
        bool window_title_visible_elsewhere = false;
        if (g.NavWindowingListWindow != NULL && (window->Flags & ImGuiWindowFlags_NoNavFocus) == 0)   // Window titles visible when using CTRL+TAB
            window_title_visible_elsewhere = true;
        if (window_title_visible_elsewhere && !window_just_created && strcmp(name, window->Name) != 0)
        {
            size_t buf_len = (size_t)window->NameBufLen;
            window->Name = ImStrdupcpy(window->Name, &buf_len, name);
            window->NameBufLen = (int)buf_len;
        }

        // UPDATE CONTENTS SIZE, UPDATE HIDDEN STATUS

        // Update contents size from last frame for auto-fitting (or use explicit size)
        const bool window_just_appearing_after_hidden_for_resize = (window->HiddenFramesCannotSkipItems > 0);
        CalcWindowContentSizes(window, &window->ContentSize, &window->ContentSizeIdeal);
        if (window->HiddenFramesCanSkipItems > 0)
            window->HiddenFramesCanSkipItems--;
        if (window->HiddenFramesCannotSkipItems > 0)
            window->HiddenFramesCannotSkipItems--;
        if (window->HiddenFramesForRenderOnly > 0)
            window->HiddenFramesForRenderOnly--;

        // Hide new windows for one frame until they calculate their size
        if (window_just_created && (!window_size_x_set_by_api || !window_size_y_set_by_api))
            window->HiddenFramesCannotSkipItems = 1;

        // Hide popup/tooltip window when re-opening while we measure size (because we recycle the windows)
        // We reset Size/ContentSize for reappearing popups/tooltips early in this function, so further code won't be tempted to use the old size.
        if (window_just_activated_by_user && (flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) != 0)
        {
            window->HiddenFramesCannotSkipItems = 1;
            if (flags & ImGuiWindowFlags_AlwaysAutoResize)
            {
                if (!window_size_x_set_by_api)
                    window->Size.x = window->SizeFull.x = 0.f;
                if (!window_size_y_set_by_api)
                    window->Size.y = window->SizeFull.y = 0.f;
                window->ContentSize = ImVec2(0.f, 0.f);
            }
        }

        // SELECT VIEWPORT
        // FIXME-VIEWPORT: In the docking/viewport branch, this is the point where we select the current viewport (which may affect the style)
        SetCurrentWindow(window);

        // LOCK BORDER SIZE AND PADDING FOR THE FRAME (so that altering them doesn't cause inconsistencies)

        if (flags & ImGuiWindowFlags_ChildWindow)
            window->WindowBorderSize = style.ChildBorderSize;
        else
            window->WindowBorderSize = ((flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) && !(flags & ImGuiWindowFlags_Modal)) ? style.PopupBorderSize : style.WindowBorderSize;
        window->WindowPadding = style.WindowPadding;
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_Popup)) && window->WindowBorderSize == 0.0f)
            window->WindowPadding = ImVec2(0.0f, (flags & ImGuiWindowFlags_MenuBar) ? style.WindowPadding.y : 0.0f);
        window->DC.MenuBarOffset.x = ImMax(ImMax(window->WindowPadding.x, style.ItemSpacing.x), g.NextWindowData.MenuBarOffsetMinVal.x);
        window->DC.MenuBarOffset.y = g.NextWindowData.MenuBarOffsetMinVal.y;

        // SCROLLING

        // Lock down maximum scrolling
        // The value of ScrollMax are ahead from ScrollbarX/ScrollbarY which is intentionally using InnerRect from previous rect in order to accommodate
        // for right/bottom aligned items without creating a scrollbar.
        window->ScrollMax.x = ImMax(0.0f, window->ContentSize.x + window->WindowPadding.x * 2.0f - window->InnerRect.GetWidth());
        window->ScrollMax.y = ImMax(0.0f, window->ContentSize.y + window->WindowPadding.y * 2.0f - window->InnerRect.GetHeight());

        // Apply scrolling
        window->Scroll = CalcNextScrollFromScrollTargetAndClamp(window);
        window->ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);

        // Setup draw list and outer clipping rectangle
        //window->DrawList->Clear();                // ???
        window->DrawList->_ResetForNewFrame();
        IM_ASSERT(window->DrawList->CmdBuffer.Size == 1 && window->DrawList->CmdBuffer[0].ElemCount == 0);
        window->DrawList->Flags = (g.Style.AntiAliasedLines ? ImDrawListFlags_AntiAliasedLines : 0) | (g.Style.AntiAliasedFill ? ImDrawListFlags_AntiAliasedFill : 0);
        window->DrawList->PushTextureID(g.Font->ContainerAtlas->TexID);

        ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)GetMainViewport();
        ImRect viewport_rect(viewport->GetMainRect());
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup) && !window_is_child_tooltip)
            PushClipRect(parent_window->ClipRect.Min, parent_window->ClipRect.Max, true);
        else
            PushClipRect(viewport_rect.Min, viewport_rect.Max, false);  // 2020/06/21: Last arg was 'true', but then no window shows up

        /*if (window_just_activated_by_user)
        {
            // Popup first latch mouse position, will position itself when it appears next frame
            window->AutoPosLastDirection = ImGuiDir_None;
            if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api)
                window->PosFloat = g.IO.MousePos;
        }*/

        // Collapse window by double-clicking on title bar
        // At this point we don't have a clipping rectangle setup yet, so we can use the title bar area for hit detection and drawing
        //int border_held = -1;
        if (!(flags & ImGuiWindowFlags_NoTitleBar))// && !(flags & ImGuiWindowFlags_NoCollapse))
        {
            ImRect title_bar_rect = window->TitleBarRect();
            /*
            if (g.HoveredWindow == window && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max) && g.IO.MouseDoubleClicked[0])
            {
                window->Collapsed = !window->Collapsed;
                if (!(flags & ImGuiWindowFlags_NoSavedSettings))
                    MarkSettingsDirty();
                FocusWindow(window);
            }
            */
            float buttonsSize = 0;
            if (p_opened)   buttonsSize+=window->TitleBarHeight() - 4.0f +3.0f;
            if (p_undocked) buttonsSize+=window->TitleBarHeight() - 4.0f +3.0f;

            const ImRect title_bar_without_buttons_rect(title_bar_rect.Min,title_bar_rect.Max-ImVec2(buttonsSize,0));

            // NEW PART: resize by dragging title bar (and mousewheel ?)
            if (wd && pDraggingStarted && (wd->dockPos==ImGui::PanelManager::BOTTOM || gImGuiDockPanelManagerAddExtraTitleBarResizing || gImGuiDockPanelManagerActiveResizeSize==0) && (*pDraggingStarted || (g.HoveredWindow == window && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max))))  {
                const ImGuiID resize_id = window->GetID("#RESIZE");
                bool hovered = false, held=false,wheel = false;//g.IO.MouseWheel!=0.f;
                if (!wheel) ButtonBehavior(title_bar_without_buttons_rect, resize_id, &hovered, &held);//, true);
                if (hovered || held || wheel) {
                    g.MouseCursor = wd->dockPos<PanelManager::TOP ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS;
                    if (!wd->isToggleWindow) wd->isResizing = true;
                }
                *pDraggingStarted = held;
                if (held || wheel)   {
                    ImVec2 newSize(0,0);
                    ImVec2 amount = wheel ? ImVec2(g.IO.MouseWheel*20.f,g.IO.MouseWheel*20.f) : g.IO.MouseDelta;
                    if (wd->dockPos==ImGui::PanelManager::LEFT || wd->dockPos==ImGui::PanelManager::TOP) newSize = ImMax(window->SizeFull + amount, style.WindowMinSize);
                    else newSize = ImMax(window->SizeFull - amount, style.WindowMinSize);
                    wd->length = wd->dockPos<ImGui::PanelManager::TOP ? newSize.x : newSize.y;
                    //border_held = 1;
                }
            }
        }
        //else window->Collapsed = false;

        const float window_rounding = window->WindowRounding;
        const float window_border_size = window->WindowBorderSize;

        // Hide new windows for one frame until they calculate their size
        if (window_just_created && (!window_size_x_set_by_api || !window_size_y_set_by_api))
            window->HiddenFramesCannotSkipItems = 1;

        // Calculate auto-fit size, handle automatic resize
        const ImVec2 size_auto_fit = CalcWindowAutoFitSize(window, window->ContentSize);
        ImVec2 size_full_modified(FLT_MAX, FLT_MAX);
        if ((flags & ImGuiWindowFlags_AlwaysAutoResize) && !window->Collapsed)
        {
            // Using SetNextWindowSize() overrides ImGuiWindowFlags_AlwaysAutoResize, so it can be used on tooltips/popups, etc.
            if (!window_size_x_set_by_api)
                window->SizeFull.x = size_full_modified.x = size_auto_fit.x;
            if (!window_size_y_set_by_api)
                window->SizeFull.y = size_full_modified.y = size_auto_fit.y;
        }
        else if (window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
        {
            // Auto-fit may only grow window during the first few frames
            // We still process initial auto-fit on collapsed windows to get a window width, but otherwise don't honor ImGuiWindowFlags_AlwaysAutoResize when collapsed.
            if (!window_size_x_set_by_api && window->AutoFitFramesX > 0)
                window->SizeFull.x = size_full_modified.x = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
            if (!window_size_y_set_by_api && window->AutoFitFramesY > 0)
                window->SizeFull.y = size_full_modified.y = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
            if (!window->Collapsed)
                MarkIniSettingsDirty(window);
        }

        // Apply minimum/maximum window size constraints and final size
        window->SizeFull = CalcWindowSizeAfterConstraint(window, window->SizeFull);
        /*window->Size = window->Collapsed ? window->TitleBarRect().GetSize() : window->SizeFull;
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup))
        {
            IM_ASSERT(window_size_x_set_by_api && window_size_y_set_by_api); // Submitted by BeginChild()
            window->Size = window->SizeFull;
        }*/
        window->Size = window->Collapsed && !(flags & ImGuiWindowFlags_ChildWindow) ? window->TitleBarRect().GetSize() : window->SizeFull;


        // POSITION

        // Popup latch its initial position, will position itself when it appears next frame
        if (window_just_activated_by_user)
        {
            window->AutoPosLastDirection = ImGuiDir_None;
            if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api)
                window->Pos = g.BeginPopupStack.back().OpenPopupPos;
        }

        // Position child window
        if (flags & ImGuiWindowFlags_ChildWindow)
        {
            IM_ASSERT(parent_window->Active);
            window->BeginOrderWithinParent = parent_window->DC.ChildWindows.Size;
            parent_window->DC.ChildWindows.push_back(window);
            if (!(flags & ImGuiWindowFlags_Popup) && !window_pos_set_by_api && !window_is_child_tooltip)
                 window->Pos = parent_window->DC.CursorPos;
        }
        //if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup) && !window_pos_set_by_api)
        //    window->Pos = window->PosFloat = parent_window->DC.CursorPos;

        const bool window_pos_with_pivot = (window->SetWindowPosVal.x != FLT_MAX && window->HiddenFramesCannotSkipItems == 0);
        if (window_pos_with_pivot)
            SetWindowPos(window, ImMax(style.DisplaySafeAreaPadding, window->SetWindowPosVal - window->SizeFull * window->SetWindowPosPivot), 0); // Position given a pivot (e.g. for centering)
        else if ((flags & ImGuiWindowFlags_ChildMenu) != 0)
            window->Pos = FindBestWindowPosForPopup(window);
        else if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api && window_just_appearing_after_hidden_for_resize)
            window->Pos = FindBestWindowPosForPopup(window);
        else if ((flags & ImGuiWindowFlags_Tooltip) != 0 && !window_pos_set_by_api && !window_is_child_tooltip)
            window->Pos = FindBestWindowPosForPopup(window);

        // Clamp position so it stays visible
        // Ignore zero-sized display explicitly to avoid losing positions if a window manager reports zero-sized window when initializing or minimizing.
        if (!window_pos_set_by_api && !(flags & ImGuiWindowFlags_ChildWindow) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
        {
            if (g.IO.DisplaySize.x > 0.0f && g.IO.DisplaySize.y > 0.0f) // Ignore zero-sized display explicitly to avoid losing positions if a window manager reports zero-sized window when initializing or minimizing.
            {
                ImVec2 padding = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
                window->Pos = ImMax(window->Pos + window->Size, padding) - window->Size;
                window->Pos = ImMin(window->Pos, g.IO.DisplaySize - padding);
            }
        }
        window->Pos = ImFloor(window->Pos);


        // SCROLLBAR VISIBILITY

        // Update scrollbar status (based on the Size that was effective during last frame or the auto-resized Size).
        if (!window->Collapsed)
        {
            // When reading the current size we need to read it after size constraints have been applied.
            // When we use InnerRect here we are intentionally reading last frame size, same for ScrollbarSizes values before we set them again.
            ImVec2 avail_size_from_current_frame = ImVec2(window->SizeFull.x, window->SizeFull.y - window->TitleBarHeight() - window->MenuBarHeight());
            ImVec2 avail_size_from_last_frame = window->InnerRect.GetSize() + window->ScrollbarSizes;
            ImVec2 needed_size_from_last_frame = window_just_created ? ImVec2(0,0) : window->ContentSize + window->WindowPadding * 2.0f;
            float size_x_for_scrollbars = (size_full_modified.x != FLT_MAX || window_just_created) ? avail_size_from_current_frame.x : avail_size_from_last_frame.x;
            float size_y_for_scrollbars = (size_full_modified.y != FLT_MAX || window_just_created) ? avail_size_from_current_frame.y : avail_size_from_last_frame.y;
            window->ScrollbarY = (flags & ImGuiWindowFlags_AlwaysVerticalScrollbar) || ((needed_size_from_last_frame.y > size_y_for_scrollbars) && !(flags & ImGuiWindowFlags_NoScrollbar));
            window->ScrollbarX = (flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar) || ((needed_size_from_last_frame.x > size_x_for_scrollbars - (window->ScrollbarY ? style.ScrollbarSize : 0.0f)) && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar));
            if (window->ScrollbarX && !window->ScrollbarY)
                window->ScrollbarY = (needed_size_from_last_frame.y > size_y_for_scrollbars) && !(flags & ImGuiWindowFlags_NoScrollbar);
            window->ScrollbarSizes = ImVec2(window->ScrollbarY ? style.ScrollbarSize : 0.0f, window->ScrollbarX ? style.ScrollbarSize : 0.0f);
        }


        // Default item width. Make it proportional to window size if window manually resizes
        if (window->Size.x > 0.0f && !(flags & ImGuiWindowFlags_Tooltip) && !(flags & ImGuiWindowFlags_AlwaysAutoResize))
            window->ItemWidthDefault = (float)(int)(window->Size.x * 0.65f);
        else
            window->ItemWidthDefault = (float)(int)(g.FontSize * 16.0f);


        // Lock window rounding for the frame (so that altering them doesn't cause inconsistencies)
        window->WindowRounding = (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildRounding : ((flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiWindowFlags_Modal)) ? style.PopupRounding : style.WindowRounding;

        // Prepare for item focus requests
        /*window->FocusIdxAllRequestCurrent = (window->FocusIdxAllRequestNext == INT_MAX || window->FocusIdxAllCounter == -1) ? INT_MAX : (window->FocusIdxAllRequestNext + (window->FocusIdxAllCounter+1)) % (window->FocusIdxAllCounter+1);
        window->FocusIdxTabRequestCurrent = (window->FocusIdxTabRequestNext == INT_MAX || window->FocusIdxTabCounter == -1) ? INT_MAX : (window->FocusIdxTabRequestNext + (window->FocusIdxTabCounter+1)) % (window->FocusIdxTabCounter+1);
        window->FocusIdxAllCounter = window->FocusIdxTabCounter = -1;
        window->FocusIdxAllRequestNext = window->FocusIdxTabRequestNext = INT_MAX;*/

        // Apply window focus (new and reactivated windows are moved to front)
        bool want_focus = false;
        if (window_just_activated_by_user && !(flags & ImGuiWindowFlags_NoFocusOnAppearing))
        {
            if (flags & ImGuiWindowFlags_Popup)
                want_focus = true;
            else if ((flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Tooltip)) == 0)
                want_focus = true;
        }

        // Draw modal window background (darkens what is behind them, all viewports)
        const bool dim_bg_for_modal = (flags & ImGuiWindowFlags_Modal) && window == GetTopMostPopupModal() && window->HiddenFramesCannotSkipItems <= 0;
        const bool dim_bg_for_window_list = g.NavWindowingTargetAnim && (window == g.NavWindowingTargetAnim->RootWindow);
        if (dim_bg_for_modal || dim_bg_for_window_list)
        {
            const ImU32 dim_bg_col = GetColorU32(dim_bg_for_modal ? ImGuiCol_ModalWindowDimBg : ImGuiCol_NavWindowingDimBg, g.DimBgRatio);
            window->DrawList->AddRectFilled(viewport_rect.Min, viewport_rect.Max, dim_bg_col);
        }

        // Draw navigation selection/windowing rectangle background
        if (dim_bg_for_window_list && window == g.NavWindowingTargetAnim)
        {
             ImRect bb = window->Rect();
             bb.Expand(g.FontSize);
             if (!bb.Contains(viewport_rect)) // Avoid drawing if the window covers all the viewport anyway
                window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(ImGuiCol_NavWindowingHighlight, g.NavWindowingHighlightAlpha * 0.25f), g.Style.WindowRounding);
        }

        // Draw window + handle manual resize
        const ImGuiWindow* window_to_highlight = g.NavWindowingTarget ? g.NavWindowingTarget : g.NavWindow;
        const bool title_bar_is_highlight = want_focus || (window_to_highlight && window->RootWindowForTitleBarHighlight == window_to_highlight->RootWindowForTitleBarHighlight);
        const ImRect title_bar_rect = window->TitleBarRect();
        if (window->Collapsed)
        {
            // Title bar only
            float backup_border_size = style.FrameBorderSize;
            g.Style.FrameBorderSize = window->WindowBorderSize;
            ImU32 title_bar_col = GetColorU32((title_bar_is_highlight && !g.NavDisableHighlight) ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBgCollapsed);
            RenderFrame(title_bar_rect.Min, title_bar_rect.Max, title_bar_col, true, window_rounding);
            g.Style.FrameBorderSize = backup_border_size;
        }
        else
        {
            //ImU32 resize_col = 0;
            //const float resize_corner_size = ImMax(g.FontSize * 1.35f, window_rounding + 1.0f + g.FontSize * 0.2f);
            /*if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && !(flags & ImGuiWindowFlags_NoResize))
            {
                // Manual resize
                const ImVec2 br = window->Rect().GetBR();
                const ImRect resize_rect(br - ImFloor(ImVec2(resize_corner_size * 0.75f, resize_corner_size * 0.75f)), br);
                const ImGuiID resize_id = window->GetID("#RESIZE");
                bool hovered, held;
                ButtonBehavior(resize_rect, resize_id, &hovered, &held, ImGuiButtonFlags_FlattenChilds);
                resize_col = GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);

                if (hovered || held)
                    g.MouseCursor = ImGuiMouseCursor_ResizeNWSE;

                if (g.HoveredWindow == window && held && g.IO.MouseDoubleClicked[0])
                {
                    // Manual auto-fit when double-clicking
                    window->SizeFull = size_auto_fit;
                    if (!(flags & ImGuiWindowFlags_NoSavedSettings))
                        MarkSettingsDirty();
                    ClearActiveID();
                }
                else if (held)
                {
                    window->SizeFull = ImMax(window->SizeFull + g.IO.MouseDelta, style.WindowMinSize);
                    if (!(flags & ImGuiWindowFlags_NoSavedSettings))
                        MarkSettingsDirty();
                }

                window->Size = window->SizeFull;
                title_bar_rect = window->TitleBarRect();
            }*/

            // NEW PART: resize by dragging window border
            if (gImGuiDockPanelManagerActiveResizeSize>0 && !(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0
                && pDraggingStarted && wd && (wd->dockPos != ImGui::PanelManager::BOTTOM || (flags & ImGuiWindowFlags_NoTitleBar))
            )
            {
                // Top windows might have a scrollbar on the bottom, and left windows on the right; so we adjust the sizes in these cases to allow dragging scrollbars instead of resizing windows
                const float resizeSizeBottomRight = gImGuiDockPanelManagerActiveResizeSize<=(style.ScrollbarSize*0.65f)?gImGuiDockPanelManagerActiveResizeSize:(style.ScrollbarSize*0.65f);
                ImVec2 rectMin(0,0),rectMax(0,0);
                if (wd->dockPos == ImGui::PanelManager::LEFT)   {
                    rectMin = window->Rect().GetTR(); rectMin.x-=resizeSizeBottomRight;rectMin.y+=title_bar_rect.GetHeight();
                    rectMax = window->Rect().GetBR();
                }
                else if (wd->dockPos == ImGui::PanelManager::RIGHT) {
                    rectMin = window->Rect().GetTL();rectMin.y+=title_bar_rect.GetHeight();
                    rectMax = window->Rect().GetBL();rectMax.x+=gImGuiDockPanelManagerActiveResizeSize;
                }
                else if (wd->dockPos == ImGui::PanelManager::TOP) {
                    rectMin = window->Rect().GetBL();rectMin.y-=resizeSizeBottomRight;
                    rectMax = window->Rect().GetBR();
                }
                else if (wd->dockPos == ImGui::PanelManager::BOTTOM) {
                    rectMin = window->Rect().GetTL();
                    rectMax = window->Rect().GetTR();rectMax.y+=gImGuiDockPanelManagerActiveResizeSize;
                }

                const ImRect resize_rect(rectMin,rectMax);
                if (*pDraggingStarted || (g.HoveredWindow == window && IsMouseHoveringRect(resize_rect.Min, resize_rect.Max)))    {
                    const ImGuiID resize_id = window->GetID("#RESIZE");
                    bool hovered = false, held=false,wheel = false;
                    if (!wheel) ButtonBehavior(resize_rect, resize_id, &hovered, &held);
                    if (hovered || held || wheel) {
                        g.MouseCursor = wd->dockPos<PanelManager::TOP ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS;
                        if (!wd->isToggleWindow) wd->isResizing = true;
                    }
                    *pDraggingStarted = held;
                    if (held || wheel)   {
                        ImVec2 newSize(0,0);
                        ImVec2 amount = wheel ? ImVec2(g.IO.MouseWheel*20.f,g.IO.MouseWheel*20.f) : g.IO.MouseDelta;
                        if (wd->dockPos==ImGui::PanelManager::LEFT || wd->dockPos==ImGui::PanelManager::TOP) newSize = ImMax(window->SizeFull + amount, style.WindowMinSize);
                        else newSize = ImMax(window->SizeFull - amount, style.WindowMinSize);
                        wd->length = wd->dockPos<ImGui::PanelManager::TOP ? newSize.x : newSize.y;
                    }
                }
                /*
                Note: recent Dear Imgui code allows resizing windows by dragging borders (don't know if single borders are supported).
                In any case ImGui::Begin() calls something like:
                UpdateManualResize(window, size_auto_fit, &border_held, resize_grip_count, &resize_grip_col[0]);
                and then:
                window->ResizeBorderHeld = (signed char)border_held;
                */
            }

            // Window background, Default alpha
            /* // old code
            ImGuiCol bg_color_idx = ImGuiCol_WindowBg;
            if ((flags & ImGuiWindowFlags_Tooltip) != 0 || (flags & ImGuiWindowFlags_Popup) != 0) //|| (flags & ImGuiWindowFlags_ComboBox) != 0*
                bg_color_idx = ImGuiCol_PopupBg;
            else if ((flags & ImGuiWindowFlags_ChildWindow) != 0)
                bg_color_idx = ImGuiCol_ChildBg;
            ImVec4 bg_color = style.Colors[bg_color_idx];
            if (bg_alpha >= 0.0f)
                bg_color.w = bg_alpha;
            bg_color.w *= style.Alpha;
            if (bg_color.w > 0.0f)
                window->DrawList->AddRectFilled(window->Pos, window->Pos+window->Size, ColorConvertFloat4ToU32(bg_color), window_rounding);
                //window->DrawList->AddRectFilled(window->Pos+ImVec2(0,window->TitleBarHeight()), window->Pos+window->Size, ColorConvertFloat4ToU32(bg_color), window_rounding, (flags & ImGuiWindowFlags_NoTitleBar) ? ImGuiCorner_All : ImGuiCorner_BottomLeft|ImGuiCorner_BottomRight);
                //window->DrawList->AddRectFilled(window->Pos + ImVec2(0, window->TitleBarHeight()), window->Pos + window->Size, bg_col, window_rounding, (flags & ImGuiWindowFlags_NoTitleBar) ? ImDrawCornerFlags_All : ImDrawCornerFlags_Bot);
            */
            // replacement
            if (!(flags & ImGuiWindowFlags_NoBackground))
            {
                ImU32 bg_col = GetColorU32(GetWindowBgColorIdxFromFlags(flags));
                float alpha = 1.0f;
                if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasBgAlpha)
                    alpha = g.NextWindowData.BgAlphaVal;
                if (alpha != 1.0f)
                    bg_col = (bg_col & ~IM_COL32_A_MASK) | (IM_F32_TO_INT8_SAT(alpha) << IM_COL32_A_SHIFT);
                window->DrawList->AddRectFilled(window->Pos + ImVec2(0, window->TitleBarHeight()), window->Pos + window->Size, bg_col, window_rounding, (flags & ImGuiWindowFlags_NoTitleBar) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersBottom);
            }


            // Title bar
            if (!(flags & ImGuiWindowFlags_NoTitleBar)) {
                //const bool window_is_focused = g.NavWindow && window->RootNonPopupWindow == g.NavWindow->RootNonPopupWindow;
                //ImU32 title_bar_col = GetColorU32(window->Collapsed ? ImGuiCol_TitleBgCollapsed : title_bar_is_highlight ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg);
                ImU32 title_bar_col = GetColorU32(title_bar_is_highlight ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg);
#               if (defined(IMGUIHELPER_H_) && !defined(NO_IMGUIHELPER_DRAW_METHODS))
                if (gImGuiDockpanelManagerExtendedStyle)    {
                    ImGui::ImDrawListAddRectWithVerticalGradient(window->DrawList,
                        title_bar_rect.GetTL(), title_bar_rect.GetBR(),title_bar_col,0.15f,0,
                        window_rounding, ImDrawFlags_RoundCornersTopLeft|ImDrawFlags_RoundCornersTopRight,1.f
                    );
                }
                else
#               endif // (defined(IMGUIHELPER_H_) && !defined(NO_IMGUIHELPER_DRAW_METHODS))
                {
                    window->DrawList->AddRectFilled(title_bar_rect.GetTL(), title_bar_rect.GetBR(), title_bar_col, window_rounding, ImDrawFlags_RoundCornersTopLeft|ImDrawFlags_RoundCornersTopRight);
                }
            }

            // Menu bar
            if (flags & ImGuiWindowFlags_MenuBar)
            {
                ImRect menu_bar_rect = window->MenuBarRect();
                menu_bar_rect.ClipWith(window->Rect());  // Soft clipping, in particular child window don't have minimum size covering the menu bar so this is useful for them.
                window->DrawList->AddRectFilled(menu_bar_rect.Min, menu_bar_rect.Max, GetColorU32(ImGuiCol_MenuBarBg), (flags & ImGuiWindowFlags_NoTitleBar) ? window_rounding : 0.0f, ImDrawFlags_RoundCornersTop);
                if (style.FrameBorderSize > 0.0f && menu_bar_rect.Max.y < window->Pos.y + window->Size.y)
                    window->DrawList->AddLine(menu_bar_rect.GetBL(), menu_bar_rect.GetBR(), GetColorU32(ImGuiCol_Border), style.FrameBorderSize);
            }

            // Scrollbars
            if (window->ScrollbarX)
                Scrollbar(ImGuiAxis_X);
            if (window->ScrollbarY)
                Scrollbar(ImGuiAxis_Y);

            // Render resize grips (after their input handling so we don't have a frame of latency)
            /*if (!(flags & ImGuiWindowFlags_NoResize))
            {
                for (int resize_grip_n = 0; resize_grip_n < resize_grip_count; resize_grip_n++)
                {
                    const ImGuiResizeGripDef& grip = resize_grip_def[resize_grip_n];
                    const ImVec2 corner = ImLerp(window->Pos, window->Pos + window->Size, grip.CornerPosN);
                    window->DrawList->PathLineTo(corner + grip.InnerDir * ((resize_grip_n & 1) ? ImVec2(window_border_size, grip_draw_size) : ImVec2(grip_draw_size, window_border_size)));
                    window->DrawList->PathLineTo(corner + grip.InnerDir * ((resize_grip_n & 1) ? ImVec2(grip_draw_size, window_border_size) : ImVec2(window_border_size, grip_draw_size)));
                    window->DrawList->PathArcToFast(ImVec2(corner.x + grip.InnerDir.x * (window_rounding + window_border_size), corner.y + grip.InnerDir.y * (window_rounding + window_border_size)), window_rounding, grip.AngleMin12, grip.AngleMax12);
                    window->DrawList->PathFillConvex(resize_grip_col[resize_grip_n]);
                }
            }*/

            // Borders
            /*if (window_border_size > 0.0f || gImGuiDockPanelManagerAlwaysDrawExternalBorders)
            {
                window->DrawList->AddRect(window->Pos+ImVec2(1,1), window->Pos+window->Size+ImVec2(1,1), GetColorU32(ImGuiCol_BorderShadow), window_rounding);
                window->DrawList->AddRect(window->Pos, window->Pos+window->Size, GetColorU32(ImGuiCol_Border), window_rounding);
                if (!(flags & ImGuiWindowFlags_NoTitleBar)) window->DrawList->AddLine(title_bar_rect.GetBL(), title_bar_rect.GetBR(), GetColorU32(ImGuiCol_Border));
            }*/
            if ((window_border_size > 0.0f && !(flags & ImGuiWindowFlags_NoBackground)) || gImGuiDockPanelManagerAlwaysDrawExternalBorders)
                window->DrawList->AddRect(window->Pos, window->Pos+window->Size, GetColorU32(ImGuiCol_Border), window_rounding, ImDrawFlags_RoundCornersAll, window_border_size);
            /*if (border_held != -1)
            {
                const float grip_draw_size = (float)(int)ImMax(g.FontSize * 1.35f, window_rounding + 1.0f + g.FontSize * 0.2f);
                ImRect border = GetBorderRect(window, border_held, grip_draw_size, 0.0f);
                window->DrawList->AddLine(border.Min, border.Max, GetColorU32(ImGuiCol_SeparatorActive), ImMax(1.0f, window_border_size));
            }
            */
            if ((style.FrameBorderSize > 0 || gImGuiDockPanelManagerAlwaysDrawExternalBorders) && !(flags & ImGuiWindowFlags_NoTitleBar))
                window->DrawList->AddLine(title_bar_rect.GetBL() + ImVec2(style.WindowBorderSize, -1), title_bar_rect.GetBR() + ImVec2(-style.WindowBorderSize,-1), GetColorU32(ImGuiCol_Border), style.FrameBorderSize);

            //RenderOuterBorders(window);  // This should replace everything from '// Borders' to this line
        }

        // Draw navigation selection/windowing rectangle border
        if (g.NavWindowingTargetAnim == window)
        {
            float rounding = ImMax(window->WindowRounding, g.Style.WindowRounding);
            ImRect bb = window->Rect();
            bb.Expand(g.FontSize);
            if (bb.Contains(viewport_rect)) // If a window fits the entire viewport, adjust its highlight inward
            {
                bb.Expand(-g.FontSize - 1.0f);
                rounding = window->WindowRounding;
            }
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_NavWindowingHighlight, g.NavWindowingHighlightAlpha), rounding, ~0, 3.0f);
        }

        // Update ContentsRegionMax. All the variable it depends on are set above in this function.
        window->ContentRegionRect.Min.x = -window->Scroll.x + window->WindowPadding.x;
        window->ContentRegionRect.Min.y = -window->Scroll.y + window->WindowPadding.y + window->TitleBarHeight() + window->MenuBarHeight();
        window->ContentRegionRect.Max.x = -window->Scroll.x - window->WindowPadding.x + (window->ContentSizeExplicit.x != 0.0f ? window->ContentSizeExplicit.x : (window->Size.x - window->ScrollbarSizes.x));
        window->ContentRegionRect.Max.y = -window->Scroll.y - window->WindowPadding.y + (window->ContentSizeExplicit.y != 0.0f ? window->ContentSizeExplicit.y : (window->Size.y - window->ScrollbarSizes.y));

        // Setup drawing context
        // (NB: That term "drawing context / DC" lost its meaning a long time ago. Initially was meant to hold transient data only. Nowadays difference between window-> and window->DC-> is dubious.)
        window->DC.Indent.x = 0.0f + window->WindowPadding.x - window->Scroll.x;
        window->DC.GroupOffset.x = 0.0f;
        window->DC.ColumnsOffset.x = 0.0f;
        window->DC.CursorStartPos = window->Pos + ImVec2(window->DC.Indent.x + window->DC.ColumnsOffset.x, window->TitleBarHeight() + window->MenuBarHeight() + window->WindowPadding.y - window->Scroll.y);
        window->DC.CursorPos = window->DC.CursorStartPos;
        window->DC.CursorPosPrevLine = window->DC.CursorPos;
        window->DC.CursorMaxPos = window->DC.CursorStartPos;
        window->DC.CurrLineSize = window->DC.PrevLineSize = ImVec2(0.0f, 0.0f);
        window->DC.CurrLineTextBaseOffset = window->DC.PrevLineTextBaseOffset = 0.0f;

        window->DC.NavHideHighlightOneFrame = false;
        window->DC.NavHasScroll = (window->ScrollMax.y > 0.0f);
        window->DC.NavLayerActiveMask = window->DC.NavLayerActiveMaskNext;
        window->DC.NavLayerActiveMaskNext = 0x00;

        window->DC.MenuBarAppending = false;
        window->DC.ChildWindows.resize(0);
        window->DC.LayoutType = ImGuiLayoutType_Vertical;
        window->DC.ParentLayoutType = parent_window ? parent_window->DC.LayoutType : ImGuiLayoutType_Vertical;
        window->DC.ItemFlags = ImGuiItemFlags_Default_;

        window->DC.ItemWidth = window->ItemWidthDefault;
        window->DC.TextWrapPos = -1.0f; // disabled
        //window->DC.ItemFlagsStack.resize(0);
        window->DC.ItemWidthStack.resize(0);
        window->DC.TextWrapPosStack.resize(0);
        window->DC.CurrentColumns = NULL;
        window->DC.TreeDepth = 0;
        window->DC.TreeJumpToParentOnPopMask = 0x00;
        window->DC.StateStorage = &window->StateStorage;
        //window->DC.GroupStack.resize(0);
        //window->DC.ItemFlags = parent_window ? parent_window->DC.ItemFlags : ImGuiItemFlags_Default_;
        //if (parent_window)  window->DC.ItemFlagsStack.push_back(window->DC.ItemFlags);

        window->DC.MenuColumns.Update(3, style.ItemSpacing.x, window_just_activated_by_user);

        if (window->AutoFitFramesX > 0)
            window->AutoFitFramesX--;
        if (window->AutoFitFramesY > 0)
            window->AutoFitFramesY--;

        // Apply focus (we need to call FocusWindow() AFTER setting DC.CursorStartPos so our initial navigation reference rectangle can start around there)
        if (want_focus) {
            FocusWindow(window);
            NavInitWindow(window, false);
        }

        // Title bar
        if (!(flags & ImGuiWindowFlags_NoTitleBar))
        {

            // Close & collapse button are on layer 1 (same as menus) and don't default focus
            const ImGuiItemFlags item_flags_backup = window->DC.ItemFlags;
            window->DC.ItemFlags |= ImGuiItemFlags_NoNavDefaultFocus;
            window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;
            window->DC.NavLayerActiveMask = (1 << ImGuiNavLayer_Menu);

            if (p_opened != NULL)   {
                //*p_opened = CloseWindowButton(p_opened);
                //const float pad = 2.0f;
                //const float rad = (window->TitleBarHeight() - pad*2.0f) * 0.5f;
                //const bool pressed = CloseButton(window->GetID("#CLOSE"), window->Rect().GetTR() + ImVec2(-pad - rad, pad + rad), rad);

                // OLD CODE:
                //const float pad = style.FramePadding.y;
                //const float rad = g.FontSize * 0.5f;
                //const bool pressed = CloseButton(window->GetID("#CLOSE"), window->Rect().GetTR() + ImVec2(-pad - rad, pad + rad), rad + 1);

                // NEW CODE:
                const float pad = style.FramePadding.x*2.f;
                const float button_sz = g.FontSize;
                const bool pressed = CloseButton(window->GetID("#CLOSE"), window->Rect().GetTR() - ImVec2(pad+button_sz,0));

                // Example of switch beetween old and new code (to keep as a reference):-------
                //const float rad = g.FontSize * 0.5f;
                //if (CloseButton(window->GetID("#CLOSE"), ImVec2(window->Pos.x + window->Size.x - style.FramePadding.x - rad, window->Pos.y + style.FramePadding.y + rad), rad + 1))
                //const float button_sz = g.FontSize;
                //if (CloseButton(window->GetID("#CLOSE"), ImVec2(window->Pos.x + window->Size.x - style.FramePadding.x * 2.0f - button_sz, window->Pos.y)))
                //-----------------------------------------------------------------------------

                if (p_opened != NULL && pressed) *p_opened = false;
                *p_opened = pressed;    // Bad but safer
            }
            if (p_undocked != NULL)
                DockWindowButton(p_undocked,p_opened);

            // Restore layer
            window->DC.NavLayerCurrent = ImGuiNavLayer_Main;
            window->DC.NavLayerActiveMask = (1 << ImGuiNavLayer_Main);
            window->DC.ItemFlags = item_flags_backup;

            // Title bar text (FIXME: refactor text alignment facilities along with RenderText helpers)
            const ImVec2 text_size = CalcTextSize(name, NULL, true);
            if (!(flags & ImGuiWindowFlags_NoCollapse))
                RenderArrow(window->DrawList,window->Pos + style.FramePadding, window->Collapsed ? ImGuiDir_Right : ImGuiDir_Down, 1.0f);

            ImVec2 text_min = window->Pos;
            ImVec2 text_max = window->Pos + ImVec2(window->Size.x, style.FramePadding.y*2 + text_size.y);
            ImRect clip_rect;
            clip_rect.Max = ImVec2(window->Pos.x + window->Size.x - (p_opened ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x) - (p_undocked ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x), text_max.y); // Match the size of CloseWindowButton()

            float pad_left = (flags & ImGuiWindowFlags_NoCollapse) == 0 ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x) : style.FramePadding.x;
            float pad_right = (p_opened != NULL) ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x) : style.FramePadding.x;
            if (p_undocked!=NULL) pad_right+=2.0f*(g.FontSize + style.ItemInnerSpacing.x);

            if (style.WindowTitleAlign.x > 0.0f) pad_right = ImLerp(pad_right, pad_left, style.WindowTitleAlign.x);
            text_min.x += pad_left;
            text_max.x -= pad_right;
            clip_rect.Min = ImVec2(text_min.x, window->Pos.y);
            RenderTextClipped(text_min, text_max, name, NULL, &text_size, ImVec2(style.WindowTitleAlign.x<0.5f?style.WindowTitleAlign.x:0.5f,style.WindowTitleAlign.y), &clip_rect);

            /*// Title bar text (with: horizontal alignment, avoiding collapse/close button, optional "unsaved document" marker)
            // FIXME: Refactor text alignment facilities along with RenderText helpers, this is too much code..
            const char* UNSAVED_DOCUMENT_MARKER = "*";
            float marker_size_x = (flags & ImGuiWindowFlags_UnsavedDocument) ? CalcTextSize(UNSAVED_DOCUMENT_MARKER, NULL, false).x : 0.0f;
            ImVec2 text_size = CalcTextSize(name, NULL, true) + ImVec2(marker_size_x, 0.0f);
            ImRect text_r = title_bar_rect;
            float pad_left = (flags & ImGuiWindowFlags_NoCollapse) ? style.FramePadding.x : (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x);
            float pad_right = (p_open == NULL)                     ? style.FramePadding.x : (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x);
            if (style.WindowTitleAlign.x > 0.0f)
                pad_right = ImLerp(pad_right, pad_left, style.WindowTitleAlign.x);
            text_r.Min.x += pad_left;
            text_r.Max.x -= pad_right;
            ImRect clip_rect = text_r;
            clip_rect.Max.x = window->Pos.x + window->Size.x - (p_open ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x); // Match the size of CloseButton()
            RenderTextClipped(text_r.Min, text_r.Max, name, NULL, &text_size, style.WindowTitleAlign, &clip_rect);
            if (flags & ImGuiWindowFlags_UnsavedDocument)
            {
                ImVec2 marker_pos = ImVec2(ImMax(text_r.Min.x, text_r.Min.x + (text_r.GetWidth() - text_size.x) * style.WindowTitleAlign.x) + text_size.x, text_r.Min.y) + ImVec2(2 - marker_size_x, 0.0f);
                ImVec2 off = ImVec2(0.0f, (float)(int)(-g.FontSize * 0.25f));
                RenderTextClipped(marker_pos + off, text_r.Max + off, UNSAVED_DOCUMENT_MARKER, NULL, NULL, ImVec2(0, style.WindowTitleAlign.y), &clip_rect);
            }*/
        }




        // Save clipped aabb so we can access it in constant-time in FindHoveredWindow()
        window->OuterRectClipped = window->Rect();
        window->OuterRectClipped.ClipWith(window->ClipRect);

        // Pressing CTRL+C while holding on a window copy its content to the clipboard
        // This works but 1. doesn't handle multiple Begin/End pairs, 2. recursing into another Begin/End pair - so we need to work that out and add better logging scope.
        // Maybe we can support CTRL+C on every element?
        /*
        if (g.ActiveId == move_id)
            if (g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_C))
                ImGui::LogToClipboard();
        */

        // Inner rectangle
        // We set this up after processing the resize grip so that our clip rectangle doesn't lag by a frame
        // Note that if our window is collapsed we will end up with a null clipping rectangle which is the correct behavior.
        window->InnerRect.Min.x = title_bar_rect.Min.x;
        window->InnerRect.Min.y = title_bar_rect.Max.y + window->MenuBarHeight();
        window->InnerRect.Max.x = window->Pos.x + window->Size.x - window->ScrollbarSizes.x;
        window->InnerRect.Max.y = window->Pos.y + window->Size.y - window->ScrollbarSizes.y;
        //window->DrawList->AddRect(window->InnerRect.Min, window->InnerRect.Max, IM_COL32_WHITE);

        // We fill last item data based on Title Bar, in order for IsItemHovered() and IsItemActive() to be usable after Begin().
        // This is useful to allow creating context menus on title bar only, etc.
        window->DC.LastItemId = window->MoveId;
        window->DC.LastItemStatusFlags = IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max, false) ? ImGuiItemStatusFlags_HoveredRect : 0;
        window->DC.LastItemRect = title_bar_rect;
    }

    // Inner clipping rectangle
    // Force round operator last to ensure that e.g. (int)(max.x-min.x) in user's render code produce correct result.
    const float border_size = window->WindowBorderSize;
    ImRect clip_rect;
    clip_rect.Min.x = ImFloor(0.5f + window->InnerRect.Min.x + ImMax(0.0f, ImFloor(window->WindowPadding.x*0.5f - border_size)));
    clip_rect.Min.y = ImFloor(0.5f + window->InnerRect.Min.y);
    clip_rect.Max.x = ImFloor(0.5f + window->InnerRect.Max.x - ImMax(0.0f, ImFloor(window->WindowPadding.x*0.5f - border_size)));
    clip_rect.Max.y = ImFloor(0.5f + window->InnerRect.Max.y);
    PushClipRect(clip_rect.Min, clip_rect.Max, true);

    // Clear 'accessed' flag last thing
    if (first_begin_of_the_frame)
        window->WriteAccessed = false;

    window->BeginCount++;
    g.NextWindowData.ClearFlags();

    // Update visibility
    if (first_begin_of_the_frame)
    {
        if (flags & ImGuiWindowFlags_ChildWindow)
        {
            // Child window can be out of sight and have "negative" clip windows.
            // Mark them as collapsed so commands are skipped earlier (we can't manually collapse them because they have no title bar).
            IM_ASSERT((flags & ImGuiWindowFlags_NoTitleBar) != 0);
            if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
                if (window->OuterRectClipped.Min.x >= window->OuterRectClipped.Max.x || window->OuterRectClipped.Min.y >= window->OuterRectClipped.Max.y)
                    window->HiddenFramesCanSkipItems = 1;

            // Hide along with parent or if parent is collapsed
            if (parent_window && (parent_window->Collapsed || parent_window->HiddenFramesCanSkipItems > 0))
                window->HiddenFramesCanSkipItems = 1;
            if (parent_window && (parent_window->Collapsed || parent_window->HiddenFramesCannotSkipItems > 0))
                window->HiddenFramesCannotSkipItems = 1;
        }

        // Don't render if style alpha is 0.0 at the time of Begin(). This is arbitrary and inconsistent but has been there for a long while (may remove at some point)
        if (style.Alpha <= 0.0f)
            window->HiddenFramesCanSkipItems = 1;

        // Update the Hidden flag
        window->Hidden = (window->HiddenFramesCanSkipItems > 0) || (window->HiddenFramesCannotSkipItems > 0);

        // Disable inputs for requested number of frames
        if (window->DisableInputsFrames > 0)
        {
            window->DisableInputsFrames--;
            window->Flags |= ImGuiWindowFlags_NoInputs;
        }

        // Update the SkipItems flag, used to early out of all items functions (no layout required)
        bool skip_items = false;
        if (window->Collapsed || !window->Active || window->Hidden)
            if (window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && window->HiddenFramesCannotSkipItems <= 0)
                skip_items = true;
        window->SkipItems = skip_items;
    }

    if (bg_alpha >= 0.0f) g.Style.Colors[bg_color_idx] = bg_color_backup;

    return !window->SkipItems;
}
static void DockWindowEnd()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (window->DC.CurrentColumns != NULL) // close columns set if any is open
        EndColumns();
    PopClipRect();   // inner window clip rectangle

    // Stop logging
    if (!(window->Flags & ImGuiWindowFlags_ChildWindow))    // FIXME: add more options for scope of logging
        ImGui::LogFinish();

    // Pop
    // NB: we don't clear 'window->RootWindow'. The pointer is allowed to live until the next call to Begin().
    g.CurrentWindowStack.pop_back();
    if (window->Flags & ImGuiWindowFlags_Popup)
        g.BeginPopupStack.pop_back();
    window->DC.StackSizesOnBegin.CompareWithCurrentState();
    SetCurrentWindow(g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back());
}

} // namespace ImGui


void ImGui::PanelManager::Pane::AssociatedWindow::updateSizeInHoverMode(const ImGui::PanelManager &mgr, const ImGui::PanelManager::Pane &pane, size_t winAndBarIndex) const  {
    if (!isValid() || !dirty) return;
    const Toolbutton* button = pane.bar.getButton(winAndBarIndex);
    const bool togglable = button && button->isToggleButton;
    if (!togglable)  {
        //fprintf(stderr,"ok \"%s\"\n",windowName);
        if (mgr.innerBarQuadSize.x<=0 || mgr.innerBarQuadSize.y<=0) mgr.calculateInnerBarQuadPlacement();
        static const float DefaultHoverSizePortion = 0.2f;
        switch (pane.pos)   {
        case TOP:
            if (sizeHoverMode<0) sizeHoverMode = ImGui::GetIO().DisplaySize.y * DefaultHoverSizePortion;
            if (sizeHoverMode<2.f * ImGui::GetStyle().WindowMinSize.x)   sizeHoverMode = 2.f * ImGui::GetStyle().WindowMinSize.x;// greater than "ImGui::GetStyle().WindowMinSize.x", otherwise the title bar is not accessible
            curPos.x = mgr.innerBarQuadPos.x;
            curPos.y = mgr.innerBarQuadPos.y;
            curSize.x = mgr.innerBarQuadSize.x;
            if (sizeHoverMode > mgr.innerBarQuadSize.y) sizeHoverMode = mgr.innerBarQuadSize.y;
            curSize.y = sizeHoverMode;
            break;
        case LEFT:
            if (sizeHoverMode<0) sizeHoverMode = ImGui::GetIO().DisplaySize.x * DefaultHoverSizePortion;
            if (sizeHoverMode<2.f * ImGui::GetStyle().WindowMinSize.x)   sizeHoverMode = 2.f * ImGui::GetStyle().WindowMinSize.x;// greater than "ImGui::GetStyle().WindowMinSize.x", otherwise the title bar is not accessible
            curPos.x = mgr.innerBarQuadPos.x;
            curPos.y = mgr.innerBarQuadPos.y;
            if (sizeHoverMode > mgr.innerBarQuadSize.x) sizeHoverMode = mgr.innerBarQuadSize.x;
            curSize.x = sizeHoverMode;
            curSize.y = mgr.innerBarQuadSize.y;
            break;
        case RIGHT:
            if (sizeHoverMode<0) sizeHoverMode = ImGui::GetIO().DisplaySize.x * DefaultHoverSizePortion;
            curPos.y = mgr.innerBarQuadPos.y;
            if (sizeHoverMode > mgr.innerBarQuadSize.x) sizeHoverMode = mgr.innerBarQuadSize.x;
            curSize.x = sizeHoverMode;
            curPos.x = mgr.innerBarQuadPos.x + mgr.innerBarQuadSize.x - curSize.x;
            curSize.y = mgr.innerBarQuadSize.y;
            break;
        case BOTTOM:
            if (sizeHoverMode<0) sizeHoverMode = ImGui::GetIO().DisplaySize.y * DefaultHoverSizePortion;
            curPos.x = mgr.innerBarQuadPos.x;
            curSize.x =  mgr.innerBarQuadSize.x;
            if (sizeHoverMode > mgr.innerBarQuadSize.y) sizeHoverMode = mgr.innerBarQuadSize.y;
            curSize.y = sizeHoverMode;
            curPos.y = mgr.innerBarQuadPos.y + mgr.innerBarQuadSize.y - curSize.y;
            break;
        default:
            IM_ASSERT("Error: unknown case\n");
            break;
        }
    }
    dirty = false;
}


void ImGui::PanelManager::Pane::AssociatedWindow::draw(const ImGui::PanelManager &mgr, const ImGui::PanelManager::Pane &pane, size_t winAndBarIndex) const  {
    if (!isValid()) return;

    const Toolbutton* button = pane.bar.getButton(winAndBarIndex);
    const bool togglable = button && button->isToggleButton;
    const bool togglableAndVisible = togglable && button->isDown;
    const bool selected = (this==pane.getSelectedWindow());
    const bool hovered = !selected && (pane.allowHoverOnTogglableWindows ? !togglableAndVisible : !togglable) && (this==pane.getHoverWindow() || persistHoverFocus);
    if (!togglableAndVisible && !selected && !hovered) return;

    if (dirty)  {
        if (selected) mgr.updateSizes();
        else if (hovered) updateSizeInHoverMode(mgr,pane,winAndBarIndex);   // (*)
    }

    if (curSize.x<0 || curSize.y<0) return;

    float& sizeToUse = hovered ? sizeHoverMode : size;
    if (selected) persistHoverFocus = false;
    //else if (hovered) ImGui::SetNextWindowFocus();  // is this line necessary ?

    const float oldSize = sizeToUse;bool open = true;bool undocked = hovered;
    WindowData wd(windowName,curPos,curSize,togglable,pane.pos,sizeToUse,open,persistHoverFocus,userData);
    //===============================================================================================
    if (wd.isToggleWindow) windowDrawer(wd);
    else if (wd.size.x>=ImGui::GetStyle().WindowMinSize.x){
        if (hovered) {
            // TO FIX: Ideally updateSizeInHoverMode(...) should not be called every frame, but it's only 1 window on screen after all
            dirty = true; // Needed for updateSizeXXX(...) to work.
            updateSizeInHoverMode(mgr,pane,winAndBarIndex); // sets dirty to false.

            // Honestly I don't know why dirty = true; is not set in some cases...
            // Note that we can comment out the line above and it's still correct,
            // because next frame (*) is triggered correctly, but there's one frame delay

            // In short: forcing "dirty = true;" every frame for the hovered windows is WRONG, but it's a hack to fix.
            // some cases in which the hovered window pos and size are wrong.
            // We should manage to detect the very first frame a new hovered window is displayed and set it dirty only on that frame.

            // I don't think the overhead is big in any case...
        }
        ImGui::SetNextWindowPos(wd.pos);
        ImGui::SetNextWindowSize(wd.size);
            static const ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings;
            ImGuiWindowFlags windowFlagsTotal = wd.isToggleWindow ? ImGuiWindowFlags_NoSavedSettings : (windowFlags|mgr.dockedWindowsExtraFlags|extraWindowFlags);
            //if (selected && !wd.isToggleWindow) windowFlagsTotal|=ImGuiWindowFlags_NoBringToFrontOnFocus;  // optional line (To be tested). [It doesn't seem to cover all cases]
            if ((windowFlagsTotal & ImGuiWindowFlags_NoTitleBar)) open=!open;   // Terrible hack to make it work (but I remind that "open" was previously called "closed": so that had a sense!). See *p_opened = CloseWindowButton(p_opened); in BeginDockWindow.
        if (ImGui::DockWindowBegin(wd.name, &wd.open,&undocked, wd.size,mgr.dockedWindowsAlpha,windowFlagsTotal,&draggingStarted,&wd))     {
            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;
            ImGuiWindow* window = NULL;
            //wd.persistFocus = ImGui::IsMouseHoveringWindow();
            //-----------------------------------------------------------------------------------------------
            windowDrawer(wd);
            //-----------------------------------------------------------------------------------------------
            window = g.CurrentWindow;
            wd.persistFocus = g.HoveredWindow ? (g.HoveredWindow->RootWindow == window || g.HoveredWindow->RootWindow == window || (hovered && wd.isResizing)) : false;
            if (hovered && !wd.persistFocus)    {
                const ImVec2& mp = ImGui::GetIO().MousePos;
                if (mp.x!=-1 && mp.y!=-1)   {
                    const ImRect bb(wd.pos,ImVec2(wd.pos.x+wd.size.x,wd.pos.y+wd.size.y));
                    wd.persistFocus = bb.Contains(mp);
                }
            }

            const bool allowManualResizeGrip = false;//true;    // I DON'T THINK THIS WORKS ANYMORE (TODO: Remove it)
            if (allowManualResizeGrip)  {
                ImRect resize_aabb(0,0,0,0);
                const ImGuiID resize_id = window->GetID("#RESIZE");
                float rounding=0; int rounding_corners=~0;
                float gripDist = 0;float gripSize = 8;
                bool hovered=false, held=false;ImVec2 newSize(-1,-1);
                const ImRect& aabb = window->Rect();
                const ImVec2& amin = aabb.Min;
                const ImVec2& amax = aabb.Max;
                switch (wd.dockPos) {
                case LEFT:
                    resize_aabb = ImRect(amax.x-gripDist-gripSize,
                                            amin.y+gripDist,
                                            amax.x-gripDist,
                                            amax.y-gripDist);
                    ButtonBehavior(resize_aabb, resize_id, &hovered, &held);//, true);
                    if (held) {
                        newSize = ImMax(window->SizeFull + g.IO.MouseDelta, style.WindowMinSize);
                        wd.length = newSize.x;
                    }
                    break;
                case RIGHT:
                    resize_aabb = ImRect(amin.x+gripDist,
                                            amin.y+gripDist,
                                            amin.x+gripDist+gripSize,
                                            amax.y-gripDist);
                    ButtonBehavior(resize_aabb, resize_id, &hovered, &held);//, true);
                    if (held) {
                        newSize = ImMax(window->SizeFull - g.IO.MouseDelta, style.WindowMinSize);
                        wd.length = newSize.x;
                    }
                    break;
                case TOP:
                    resize_aabb = ImRect(amin.x+gripDist,
                                            amax.y-gripDist-gripSize,
                                            amax.x-gripDist,
                                            amax.y-gripDist);
                    ButtonBehavior(resize_aabb, resize_id, &hovered, &held);//, true);
                    if (held) {
                        newSize = ImMax(window->SizeFull + g.IO.MouseDelta, style.WindowMinSize);
                        wd.length = newSize.y;
                    }
                    break;
                case BOTTOM:
                    resize_aabb = ImRect(amin.x+gripDist,
                                            amin.y+gripDist,
                                            amax.x-gripDist,
                                            amin.y+gripDist+gripSize);
                    ButtonBehavior(resize_aabb, resize_id, &hovered, &held);//, true);
                    if (held) {
                        newSize = ImMax(window->SizeFull - g.IO.MouseDelta, style.WindowMinSize);
                        wd.length = newSize.y;
                    }
                    break;
                default:
                    IM_ASSERT("Error");
                    break;
                }

                if (hovered || held)    {
                    const ImGuiCol resize_col = GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);
                    window->DrawList->AddRectFilled(resize_aabb.Min, resize_aabb.Max, resize_col, rounding, rounding_corners);
                }
                if (held) wd.persistFocus = true;

            }

            if (sizeToUse!=oldSize) {
                dirty = true;
                // Better clamp size now if it's a "selected" window
                if (selected)   {
                    if (wd.dockPos<TOP) {
                        const float delta = mgr.innerQuadSize.x + oldSize;
                        if (sizeToUse>delta)  sizeToUse = delta;
                        const float minWidth = 2.f * ImGui::GetStyle().WindowMinSize.x; // greater than "ImGui::GetStyle().WindowMinSize.x", otherwise the title bar is not accessible
                        if (sizeToUse<minWidth) sizeToUse = minWidth;
                        curSize.x = sizeToUse;
                    }
                    else {
                        const float delta = mgr.innerQuadSize.y + oldSize;
                        if (sizeToUse>delta)  sizeToUse = delta;
                        curSize.y = sizeToUse;
                    }
                }
            }

        }
        ImGui::DockWindowEnd();
    }
    //================================================================================================
    if (open) {
        if (hovered) {
            persistHoverFocus = false;
        }
        //else if (togglableAndVisible) button->isDown = false;
        else if (selected) {
            pane.bar.setSelectedButtonIndex(-1);
            persistHoverFocus = false;  // optional line (when commented out leaves a selected window in hover move: we must press the close button another time to hide it)
            dirty = true;mgr.updateSizes();  // Needed to update the innerQuadSize
            dirty = true; // Needed so next time the hover window sizes are set
        }
    }
    else if (togglableAndVisible) button->isDown = false;
    if (!wd.isToggleWindow) {
        if (!undocked && hovered)   {
            pane.bar.setSelectedButtonIndex(winAndBarIndex);
            dirty = true;
        }
        else if (undocked && selected)   {
            pane.bar.setSelectedButtonIndex(-1);
            persistHoverFocus = true;  // optional line (when commented out leaves a selected window in hover move: we must press the close button another time to hide it)
            dirty = true; mgr.updateSizes();  // Needed to update the innerQuadSize
            dirty = true; // Needed so next time the hover window sizes are set
        }
    }
}


size_t ImGui::PanelManager::Pane::addButtonAndWindow(const ImGui::Toolbutton &button, const ImGui::PanelManager::Pane::AssociatedWindow &window, int insertPosition) {
    int numButtons = (int) bar.getNumButtons();
    if (numButtons>0 && insertPosition>=0 && insertPosition<numButtons)    {
        bar.addButton(bar.buttons[numButtons-1]);
        windows.push_back(windows[numButtons-1]);
        if (bar.selectedButtonIndex==numButtons-1) bar.selectedButtonIndex=numButtons;
        if (bar.hoverButtonIndex==numButtons-1) bar.hoverButtonIndex=numButtons;
        for (int i=numButtons-1;i>=insertPosition;i--)    {
            bar.buttons[i+1]=bar.buttons[i];
            windows[i+1]=windows[i];
            if (bar.selectedButtonIndex==i) bar.selectedButtonIndex=i+1;
            if (bar.hoverButtonIndex==i) bar.hoverButtonIndex=i+1;
        }
        bar.buttons[insertPosition]=button;
        windows[insertPosition]=window;
    }
    else {
        bar.addButton(button);
	windows.push_back(window);
	/*const int wsz = (int) windows.size();
	windows.resize(wsz+1);
	windows[wsz] = window;*/
    }
    bar.updatePositionAndSize();
    return getSize();
}


size_t ImGui::PanelManager::Pane::addSeparator(float pixels, int insertPosition)  {
    return addButtonOnly(Toolbutton("",NULL,ImVec2(0,0),ImVec2(0,0),ImVec2(pixels,pixels)),insertPosition);
}

size_t ImGui::PanelManager::Pane::addButtonOnly(const Toolbutton& button, int insertPosition)  {
    return addButtonAndWindow(button,AssociatedWindow(),insertPosition);
}


size_t ImGui::PanelManager::Pane::addClonedButtonAndWindow(const ImGui::PanelManager::Pane &sourcePane, const size_t sourcePaneEntryIndex, bool flipButtonHorizontally)   {
    IM_ASSERT(&sourcePane!=this);
    IM_ASSERT(sourcePane.getSize()>sourcePaneEntryIndex);
    const Toolbutton& sourceButton = *sourcePane.bar.getButton(sourcePaneEntryIndex);
    const Pane::AssociatedWindow& sourceWindow = sourcePane.windows[sourcePaneEntryIndex];
    IM_ASSERT(!sourceButton.isToggleButton);    // cloning toggle buttons is NOT supported
    if (sourceButton.isToggleButton) return getSize();
    size_t size = getSize();
    bar.addButton(sourceButton);
    windows.push_back(sourceWindow);
    if (flipButtonHorizontally) {
        Toolbutton& b = *bar.getButton(size);
        const float tmp = b.uv0.x;b.uv0.x=b.uv1.x;b.uv1.x=tmp;
    }
    bar.updatePositionAndSize();
    return size+1;
}


size_t ImGui::PanelManager::Pane::addClonedPane(const ImGui::PanelManager::Pane &sourcePane, bool flipButtonHorizontally, int sourcePaneEntryStartIndex, int sourcePaneEntryEndIndex)  {
    const int sourcePaneSize = (int)sourcePane.getSize();
    if (sourcePaneSize==0) return getSize();
    sourcePaneEntryEndIndex = (sourcePaneEntryEndIndex<0 || sourcePaneEntryEndIndex>=sourcePaneSize) ? (sourcePaneSize-1) : sourcePaneEntryEndIndex;
    sourcePaneEntryStartIndex = sourcePaneEntryStartIndex<0 ? 0 : sourcePaneEntryStartIndex>sourcePaneEntryEndIndex ? sourcePaneEntryEndIndex : sourcePaneEntryStartIndex;
    for (int i=sourcePaneEntryStartIndex;i<=sourcePaneEntryEndIndex;i++) {
        if (sourcePane.bar.getButton(i)->isToggleButton) continue;   // Not supported
        addClonedButtonAndWindow(sourcePane,i,flipButtonHorizontally);
    }
    return getSize();
}

size_t ImGui::PanelManager::Pane::deleteButtonAt(int index)   {
    int numButtons = (int) bar.getNumButtons();
    if (numButtons==0 || index<0 || index>=numButtons) return getSize();
    if (bar.selectedButtonIndex==index) bar.selectedButtonIndex=-1;
    if (bar.hoverButtonIndex==index) bar.hoverButtonIndex=-1;
    for (int i=index;i<numButtons-1;i++)    {
        bar.buttons[i]=bar.buttons[i+1];
        windows[i]=windows[i+1];
        if (bar.selectedButtonIndex==i+1) bar.selectedButtonIndex=i;
        if (bar.hoverButtonIndex==i+1) bar.hoverButtonIndex=i;
    }
    --numButtons;
    bar.buttons[numButtons].~Button();
    bar.buttons.resize(numButtons);
    windows.resize(numButtons);
    bar.updatePositionAndSize();
    return getSize();
}


size_t ImGui::PanelManager::Pane::getSize() const {
    IM_ASSERT((int)bar.getNumButtons()==windows.size());
    return windows.size();
}

void ImGui::PanelManager::Pane::setToolbarProperties(bool _keepAButtonSelected, bool _lightAllBarWhenHovered, const ImVec2 &_hvAlignmentsIn01, const ImVec2 &_opacityOffAndOn, const ImVec4 &_bg_col, const ImVec4 &_displayPortion) {
    bool _vertical = pos < TOP;
    ImVec2 al = _hvAlignmentsIn01;
    if (al.x==-1000)    al.x = (pos>=TOP) ? 0.5f : pos == RIGHT ? 1.0f : 0.f;
    if (al.y==-1000)    al.y = (pos<TOP) ? 0.0f : pos == BOTTOM ? 1.0f : 0.f;
    bar.setProperties(_keepAButtonSelected,_vertical,_lightAllBarWhenHovered,al,_opacityOffAndOn,_bg_col,_displayPortion);
}

void ImGui::PanelManager::Pane::setDisplayProperties(const ImVec2 &_opacityOffAndOn, const ImVec4 &_bg_col) {
    bar.setDisplayProperties(_opacityOffAndOn,_bg_col);
}

bool ImGui::PanelManager::Pane::isButtonPressed(int index) const   {
    const Toolbutton* button = bar.getButton(index);
    if (!button) return false;
    if (button->isToggleButton) return button->isDown;
    const int selectedIndex = bar.getSelectedButtonIndex();
    return (selectedIndex==index);
}

bool ImGui::PanelManager::Pane::setButtonPressed(int index, bool flag)   {
    Toolbutton* button = bar.getButton(index);
    if (!button) return false;
    AssociatedWindow& wnd = windows[index];
    if (!button->isToggleButton) {
        if (!bar.keepAButtonSelected) return false;
        if (bar.getSelectedButtonIndex()!=index) bar.setSelectedButtonIndex(index);
        if (wnd.isValid()) wnd.dirty = true;    // Must we include this in the "if" above ?
    }
    else {
        if (button->isDown != flag) {
            button->isDown = flag;
            //if (wnd.isValid()) {wnd.}   //  Must we do something here ?
        }
    }
    return true;
}

const char *ImGui::PanelManager::Pane::getWindowName(int index) const  {
    if (index<0 || index>=(int)windows.size()) return NULL;
    return windows[index].windowName;
}

int ImGui::PanelManager::Pane::findWindowIndex(const char *windowName) const    {
    if (!windowName) return -1;
    for (size_t i=0,isz=windows.size();i<isz;i++) {
        const char* wn = windows[i].windowName;
        if (strcmp(windowName,wn)==0) return (int)i;
    }
    return -1;
}

void ImGui::PanelManager::Pane::getButtonAndWindow(size_t index, ImGui::Toolbutton **pToolbutton, ImGui::PanelManager::Pane::AssociatedWindow **pAssociatedWindow)  // Returns references
{
    if (pToolbutton) *pToolbutton=bar.getButton(index);
    if (pAssociatedWindow)  {
        if ((int)index<windows.size()) *pAssociatedWindow=NULL;
        else *pAssociatedWindow=&windows[index];
    }
}

void ImGui::PanelManager::Pane::getButtonAndWindow(size_t index, const ImGui::Toolbutton **pToolbutton, const ImGui::PanelManager::Pane::AssociatedWindow **pAssociatedWindow) const // Returns references
{
    if (pToolbutton) *pToolbutton=bar.getButton(index);
    if (pAssociatedWindow)  {
        if ((int)index<windows.size()) *pAssociatedWindow=NULL;
        else *pAssociatedWindow=&windows[index];
    }
}

void ImGui::PanelManager::Pane::getButtonAndWindow(const char *windowName, ImGui::Toolbutton **pToolbutton, ImGui::PanelManager::Pane::AssociatedWindow **pAssociatedWindow)  // Returns references
{
    const int index = findWindowIndex(windowName);
    if (index==-1) {
        if (pToolbutton) *pToolbutton=NULL;
        if (pAssociatedWindow) *pAssociatedWindow=NULL;
    }
    else getButtonAndWindow((size_t)index,pToolbutton,pAssociatedWindow);
}

void ImGui::PanelManager::Pane::getButtonAndWindow(const char *windowName, const ImGui::Toolbutton **pToolbutton, const ImGui::PanelManager::Pane::AssociatedWindow **pAssociatedWindow) const // Returns references
{
    const int index = findWindowIndex(windowName);
    if (index==-1) {
        if (pToolbutton) *pToolbutton=NULL;
        if (pAssociatedWindow) *pAssociatedWindow=NULL;
    }
    else getButtonAndWindow((size_t)index,pToolbutton,pAssociatedWindow);
}


ImGui::PanelManager::Pane *ImGui::PanelManager::addPane(ImGui::PanelManager::Position pos, const char *toolbarName) {
    const size_t sz = panes.size();
    for (size_t i=0;i<sz;i++)   {
        IM_ASSERT(panes[i].pos!=pos);   // ONLY ONE PANE PER SIDE IS SUPPORTED
        if (panes[i].pos==pos) return NULL;
    }
    panes.push_back(Pane(pos,toolbarName));
    Pane& pane = panes[sz];
    switch (pos)    {
    case LEFT:      paneLeft = &pane;break;
    case RIGHT:     paneRight = &pane;break;
    case TOP:       paneTop = &pane;break;
    case BOTTOM:    paneBottom = &pane;break;
    default:
        IM_ASSERT(false);   //Unknown pos
        break;
    }
    pane.setToolbarProperties();
    return &pane;
}


bool ImGui::PanelManager::render(Pane** pPanePressedOut, int *pPaneToolbuttonPressedIndexOut) const   {
    if (!visible) return false;

    const ImVec2 oldInnerQuadPos = innerQuadPos;
    const ImVec2 oldInnerQuadSize = innerQuadSize;

    if (pPanePressedOut) *pPanePressedOut=NULL;
    if (pPaneToolbuttonPressedIndexOut) *pPaneToolbuttonPressedIndexOut=-1;

    // Hack to update sizes at the beginning:
    if (innerBarQuadSize.x==ImGui::GetIO().DisplaySize.x && innerBarQuadSize.y==ImGui::GetIO().DisplaySize.y) {
        if (!isEmpty()) {
            calculateInnerBarQuadPlacement();
            updateSizes();
        }
    }

    bool mustUpdateSizes = false;

    // Fill mustUpdateSizes:
    for (size_t paneIndex=0,panesSize=(size_t)panes.size();paneIndex<panesSize;paneIndex++) {
        const Pane& pane = panes[paneIndex];
        if (!pane.visible) continue;
        const Toolbar& bar = pane.bar;

        const int oldHoverButtonIndex = bar.getHoverButtonIndex();
        const int oldSelectedButtonIndex = bar.getSelectedButtonIndex();
        const int pressed = bar.render(true);   // "true" uses ImGui::IsItemHoveringRect()
        int selectedButtonIndex = bar.getSelectedButtonIndex();
        int& hoverButtonIndex = bar.hoverButtonIndex;

        bool selectedButtonIndexChanged = (oldSelectedButtonIndex!=selectedButtonIndex);
        mustUpdateSizes|=selectedButtonIndexChanged;

        bool hoverButtonIndexChanged = (oldHoverButtonIndex!=hoverButtonIndex);

        // new selected toolbuttons without valid windows should act like manual button (i.e. not take selection):
        if (selectedButtonIndexChanged && selectedButtonIndex>=0 && selectedButtonIndex<(int)bar.getNumButtons()) {
            const Pane::AssociatedWindow& wnd = pane.windows[selectedButtonIndex];
            if (!wnd.isValid()) {
                bar.selectedButtonIndex = selectedButtonIndex = oldSelectedButtonIndex;
                selectedButtonIndexChanged = false;
                if (selectedButtonIndex!=-1) updateSizes();
                //fprintf(stderr,"Pressed NULL window\n");
            }
        }

        // Optional hack: more comfortable hovering from button to window without easily losing focus
        if (hoverButtonIndexChanged && !selectedButtonIndexChanged && hoverButtonIndex==-1) {
            if (oldHoverButtonIndex<(int)bar.getNumButtons())    {
                const Pane::AssociatedWindow& window = pane.windows[oldHoverButtonIndex];
                if (window.isValid())
                {
                    if (!window.persistHoverFocus && pane.hoverReleaseIndex!=oldHoverButtonIndex) {
                        pane.hoverReleaseIndex=oldHoverButtonIndex;
                        pane.hoverReleaseTimer=ImGui::GetTime();
                        //fprintf(stderr,"Starting timer\n");

                        hoverButtonIndex = oldHoverButtonIndex;
                        hoverButtonIndexChanged = false;
                        window.persistHoverFocus = true;
                    }
                    else if (pane.hoverReleaseIndex == oldHoverButtonIndex)
                    {
                        if (ImGui::GetTime()-pane.hoverReleaseTimer<0.25f)   {
                            hoverButtonIndex = oldHoverButtonIndex;
                            hoverButtonIndexChanged = false;
                            window.persistHoverFocus = true;
                            //fprintf(stderr,"Keeping focus\n");
                        }
                        else {
                            pane.hoverReleaseIndex = -1;
                            pane.hoverReleaseTimer = -1.f;
                            //fprintf(stderr,"Hovering timer elapsed\n");
                        }
                    }
                }
            }
        }

        if (pressed>=0)    {
            // Fill Optional arguments
            if (pPanePressedOut) *pPanePressedOut=const_cast<Pane*> (&pane);
            if (pPaneToolbuttonPressedIndexOut) *pPaneToolbuttonPressedIndexOut=(int) pressed;
        }

        // hack (without this block when clicking to a selected toolbutton, the hover window does not enlarge its size (like it does when clicking on its titlebar button)
        if (selectedButtonIndexChanged && selectedButtonIndex==-1 && hoverButtonIndex>=0 && hoverButtonIndex==oldSelectedButtonIndex)   {
            const Toolbutton* button = bar.getButton((size_t)hoverButtonIndex);
            if (!button->isToggleButton)    {
                pane.windows[hoverButtonIndex].dirty = true;
            }
        }

        // Cross-pane window handling (optional):
        {
            if (selectedButtonIndexChanged && selectedButtonIndex>=0)   {
                const char* windowName = pane.windows[selectedButtonIndex].windowName;
                if (!windowName) continue;
                // We must turn off the newly selected windows from other panes if present:
                for (size_t pi=0,panesSize=(size_t)panes.size();pi<panesSize;pi++) {
                    if (pi==paneIndex) continue;    // this pane
                    const Pane& pn = panes[pi];
                    const int si = pn.getSelectedIndex();
                    if (si>=0)  {
                        const char* cmpName = pn.windows[si].windowName;
                        if (cmpName && strcmp(windowName,cmpName)==0)   pn.bar.setSelectedButtonIndex(-1);
                    }
                }

            }
            else if (hoverButtonIndexChanged && hoverButtonIndex>=0 && hoverButtonIndex!=selectedButtonIndex)    {
                const Toolbutton* button = bar.getButton((size_t)hoverButtonIndex);
                if (!button->isToggleButton)    {
                    const char* windowName = pane.windows[hoverButtonIndex].windowName;
                    // we must avoid hovering if "windowName" is selected on another pane
                    if (windowName) {
                        for (size_t pi=0,panesSize=(size_t)panes.size();pi<panesSize;pi++) {
                            if (pi==paneIndex) continue;    // this pane
                            const Pane& pn = panes[pi];
                            const int si = pn.getSelectedIndex();
                            if (si>=0)  {
                                const char* cmpName = pn.windows[si].windowName;
                                if (cmpName && strcmp(windowName,cmpName)==0)   pane.bar.hoverButtonIndex = -1;
                            }
                        }
                    }
                }
            }
        }

    }

    if (mustUpdateSizes) this->updateSizes();

    // Draw windows:
    for (size_t paneIndex=0,panesSize=(size_t)panes.size();paneIndex<panesSize;paneIndex++) {
        const Pane& pane = panes[paneIndex];
        if (!pane.visible) continue;
        for (size_t windowIndex=0,paneWindowsSize=(size_t)pane.windows.size();windowIndex<paneWindowsSize;windowIndex++)    {
            const Pane::AssociatedWindow& window = pane.windows[windowIndex];
            window.draw(*this,pane,windowIndex);
        }
    }


    const bool innerQuadChanged = (innerQuadPos.x!=oldInnerQuadPos.x || innerQuadPos.y!=oldInnerQuadPos.y ||
                                      innerQuadSize.x!=oldInnerQuadSize.x || innerQuadSize.y!=oldInnerQuadSize.y);
    if (innerQuadChanged)    {
        if (innerQuadChangedTimer<0) innerQuadChangedTimer = ImGui::GetTime();
    }
    else if (innerQuadChangedTimer>0 && ImGui::GetTime()-innerQuadChangedTimer>1.f)    {
        innerQuadChangedTimer = -1.f;
        return true;
    }

    return false;
}


size_t ImGui::PanelManager::getNumPanes() const {return panes.size();}

const ImGui::PanelManager::Pane *ImGui::PanelManager::getPane(ImGui::PanelManager::Position pos) const {
    for (size_t i=0,isz=panes.size();i<isz;i++) {
        if (panes[i].pos==pos) return &panes[i];
    }
    return NULL;
}

ImGui::PanelManager::Pane *ImGui::PanelManager::getPane(ImGui::PanelManager::Position pos)  {
    for (size_t i=0,isz=panes.size();i<isz;i++) {
        if (panes[i].pos==pos) return &panes[i];
    }
    return NULL;
}


void ImGui::PanelManager::setToolbarsScaling(float scalingX, float scalingY) {
    for (size_t i=0,isz=panes.size();i<isz;i++) panes[i].setToolbarScaling(scalingX,scalingY);
    calculateInnerBarQuadPlacement();
    updateSizes();

    // Set all windows "dirty" and clamp visible "toggle-windows" to the display size (although in the latter case "win.curSize" is not used at all [ImGui::GetWindowSize(name) does not exist ATM])
    for (size_t pi=0,pisz=panes.size();pi<pisz;pi++) {
        const Pane& pane = panes[pi];
        for (size_t i=0,isz=pane.windows.size();i<isz;i++) {
            const Pane::AssociatedWindow& win = pane.windows[i];
            if (win.isValid()) {
                win.dirty=true;

                const Toolbutton& but = pane.bar.buttons[i];
                if (but.isToggleButton) {
                    // simply clamp it
                    if (win.curPos.x+win.curSize.x>innerBarQuadPos.x+innerQuadSize.x)   win.curPos.x=innerBarQuadPos.x+innerQuadSize.x-win.curSize.x;
                    if (win.curPos.y+win.curSize.y>innerBarQuadPos.y+innerQuadSize.y)   win.curPos.y=innerBarQuadPos.y+innerQuadSize.y-win.curSize.y;
                    if (win.curPos.x<innerBarQuadPos.x)     win.curPos.x=innerBarQuadPos.x;
                    if (win.curPos.y<innerBarQuadPos.y)     win.curPos.y=innerBarQuadPos.y;
                }
            }
        }
    }
}

void ImGui::PanelManager::overrideAllExistingPanesDisplayProperties(const ImVec2 &_opacityOffAndOn, const ImVec4 &_bg_col)  {
    for (size_t pi=0,pisz=panes.size();pi<pisz;pi++) {
        Pane& pane = panes[pi];
        pane.setDisplayProperties(_opacityOffAndOn,_bg_col);
    }
}

void ImGui::PanelManager::updateSizes() const  {
    if (innerBarQuadSize.x<=0 || innerBarQuadSize.y<=0) calculateInnerBarQuadPlacement();
    //
    //
    innerQuadPos = innerBarQuadPos;
    innerQuadSize = innerBarQuadSize;
    const ImVec2& winMinSize = ImGui::GetStyle().WindowMinSize;

    static const float DefaultDockSizePortion = 0.25f;
    if (paneTop)    {
        const Pane& pane = *paneTop;
        const int selectedButtonIndex = pane.bar.getSelectedButtonIndex();
        //
        if (selectedButtonIndex>=0) {
            const Pane::AssociatedWindow& win = pane.windows[selectedButtonIndex];
            if (win.isValid())  {
                //
                win.curPos.x = innerBarQuadPos.x;
                win.curPos.y = innerBarQuadPos.y;
                win.curSize.x = innerBarQuadSize.x;
                if (win.size<0) win.size = ImGui::GetIO().DisplaySize.y * DefaultDockSizePortion;
                else if (win.size<winMinSize.y) win.size=winMinSize.y;
                if (win.size>innerBarQuadSize.y)  win.size = innerBarQuadSize.y;
                win.curSize.y = win.size;
                //
                innerQuadPos.y+=win.curSize.y;
                innerQuadSize.y-=win.curSize.y;
                //
            }            
            win.dirty = false;
        }
    }
    if (paneLeft)    {
        const Pane& pane = *paneLeft;
        const int selectedButtonIndex = pane.bar.getSelectedButtonIndex();
        //
        if (selectedButtonIndex>=0) {
            const Pane::AssociatedWindow& win = pane.windows[selectedButtonIndex];
            if (win.isValid())  {
                //
                win.curPos.x = innerBarQuadPos.x;
                win.curPos.y = innerQuadPos.y;
                if (win.size<0) win.size = ImGui::GetIO().DisplaySize.x * DefaultDockSizePortion;
                else if (win.size<winMinSize.x) win.size=winMinSize.x;
                if (win.size>innerBarQuadSize.x)  win.size = innerBarQuadSize.x;
                win.curSize.x = win.size;
                win.curSize.y = innerQuadSize.y;

                //
                innerQuadPos.x+=win.curSize.x;
                innerQuadSize.x-=win.curSize.x;
                //
            }
            win.dirty = false;
        }
    }
    if (paneRight)    {
        const Pane& pane = *paneRight;
        const int selectedButtonIndex = pane.bar.getSelectedButtonIndex();
        //
        if (selectedButtonIndex>=0) {
            const Pane::AssociatedWindow& win = pane.windows[selectedButtonIndex];
            if (win.isValid())  {
                //
                win.curPos.y = innerQuadPos.y;
                win.curSize.y = innerQuadSize.y;
                if (win.size<0) win.size = ImGui::GetIO().DisplaySize.x * DefaultDockSizePortion;
                else if (win.size<winMinSize.x) win.size=winMinSize.x;
                if (win.size>innerQuadSize.x)  win.size = innerQuadSize.x;
                win.curSize.x = win.size;
                win.curPos.x = innerBarQuadPos.x+innerBarQuadSize.x-win.curSize.x;
                //
                innerQuadSize.x-=win.curSize.x;
                //
            }
            win.dirty = false;
        }
    }
    if (paneBottom)    {
        const Pane& pane = *paneBottom;
        const int selectedButtonIndex = pane.bar.getSelectedButtonIndex();
        //
        if (selectedButtonIndex>=0) {
            const Pane::AssociatedWindow& win = pane.windows[selectedButtonIndex];
            if (win.isValid())  {
                //
                win.curPos.x = innerQuadPos.x;
                win.curSize.x = innerQuadSize.x;
                if (win.size<0) win.size = ImGui::GetIO().DisplaySize.y * DefaultDockSizePortion;
                else if (win.size<winMinSize.y) win.size=winMinSize.y;
                if (win.size>innerQuadSize.y)  win.size = innerQuadSize.y;
                win.curSize.y = win.size;
                win.curPos.y = innerQuadPos.y+innerQuadSize.y-win.curSize.y;
                //
                innerQuadSize.y-=win.curSize.y;
                //
            }
            win.dirty = false;
        }
    }

    //fprintf(stderr,"innerBarQuad  (%1.1f,%1.1f,%1.1f,%1.1f)\n",innerBarQuadPos.x,innerBarQuadPos.y,innerBarQuadSize.x,innerBarQuadSize.y);
    //fprintf(stderr,"innerQuad     (%1.1f,%1.1f,%1.1f,%1.1f)\n",innerQuadPos.x,innerQuadPos.y,innerQuadSize.x,innerQuadSize.y);
}

void ImGui::PanelManager::closeHoverWindow() {
    for (int i=0,isz=getNumPanes();i<isz;i++)   {
        Pane& pane = panes[i];
        Toolbar& bar = pane.bar;
        for (int w=0,wsz=bar.buttons.size();w<wsz;w++)  {
            Pane::AssociatedWindow& window = pane.windows[w];
            if (w == bar.hoverButtonIndex)    {
                // The next two lines were outside the loop
                window.dirty = true;
                window.updateSizeInHoverMode(*this,pane,w);
                window.dirty = true;
                window.persistHoverFocus = false;
                bar.hoverButtonIndex=-1;
            }
        }
    }
}

const ImVec2& ImGui::PanelManager::getCentralQuadPosition() const   {
    return innerQuadPos;
}
const ImVec2& ImGui::PanelManager::getCentralQuadSize() const   {
    return innerQuadSize;
}

const ImVec2& ImGui::PanelManager::getToolbarCentralQuadPosition() const    {
    return innerBarQuadPos;
}
const ImVec2& ImGui::PanelManager::getToolbarCentralQuadSize() const    {
    return innerBarQuadSize;
}

void ImGui::PanelManager::setDisplayPortion(const ImVec4 &_displayPortion) {
    for (size_t i=0,isz=panes.size();i<isz;i++) {
        Pane& p = panes[i];
        p.bar.setDisplayPortion(_displayPortion);
    }
    innerBarQuadSize.x=innerBarQuadSize.y=0;    // forces update
    updateSizes();
}



void ImGui::PanelManager::calculateInnerBarQuadPlacement() const {
    // Calculates "innerBarQuadPos" and "innerBarQuadSize" (this changes only after a screen resize)
    innerBarQuadPos = ImVec2(0,0);
    innerBarQuadSize = ImGui::GetIO().DisplaySize;
    const float deltaPixels = 2;
    if (paneTop)    {
        const Pane& pane = *paneTop;
        const ImVec2& pos  = pane.bar.toolbarWindowPos;
        const ImVec2& size = pane.bar.toolbarWindowSize;
        innerBarQuadPos.y = pos.y + size.y - deltaPixels;
        innerBarQuadSize.y-=innerBarQuadPos.y;
    }
    if (paneBottom)    {
        const Pane& pane = *paneBottom;
        const ImVec2& size = pane.bar.toolbarWindowSize;
        innerBarQuadSize.y-=size.y;innerBarQuadSize.y+=deltaPixels;
    }
    if (paneLeft)    {
        const Pane& pane = *paneLeft;
        const ImVec2& pos  = pane.bar.toolbarWindowPos;
        const ImVec2& size = pane.bar.toolbarWindowSize;
        innerBarQuadPos.x = pos.x + size.x - deltaPixels;
        innerBarQuadSize.x-=innerBarQuadPos.x;
    }
    if (paneRight)    {
        const Pane& pane = *paneRight;
        const ImVec2& size = pane.bar.toolbarWindowSize;
        innerBarQuadSize.x-=size.x;innerBarQuadSize.x+=deltaPixels;
    }
    //fprintf(stderr,"innerBarQuad  (%1.1f,%1.1f,%1.1f,%1.1f)\n",innerBarQuadPos.x,innerBarQuadPos.y,innerBarQuadSize.x,innerBarQuadSize.y);
}



#if (defined(IMGUIHELPER_H_) && !defined(NO_IMGUIHELPER_SERIALIZATION))
#ifndef NO_IMGUIHELPER_SERIALIZATION_SAVE
#include "../imguihelper/imguihelper.h"
bool ImGui::PanelManager::Save(const PanelManager &mgr, ImGuiHelper::Serializer& s) {
    if (!s.isValid()) return false;

    const ImVec2& displaySize = ImGui::GetIO().DisplaySize;
    IM_ASSERT(displaySize.x>=0 && displaySize.y>=0);

    int mwf = (int) mgr.getDockedWindowsExtraFlags();s.save(ImGui::FT_ENUM,&mwf,"managerExtraWindowFlags");
    int sz = mgr.panes.size();s.save(&sz,"numPanes");
    for (int ip=0,ipsz=mgr.panes.size();ip<ipsz;ip++)    {
        const ImGui::PanelManager::Pane& pane = mgr.panes[ip];
        int pos = (int)pane.pos;s.save(ImGui::FT_ENUM,&pos,"pos");
        s.save(&pane.bar.selectedButtonIndex,"selectedButtonIndex");
        IM_ASSERT(pane.bar.buttons.size()==pane.windows.size());
        sz = pane.bar.buttons.size();s.save(&sz,"numPaneButtons");
        for (int ib=0,ibsz=pane.bar.buttons.size();ib<ibsz;ib++)    {
            const ImGui::Toolbar::Button& button = pane.bar.buttons[ib];
            const ImGui::PanelManager::Pane::AssociatedWindow& window = pane.windows[ib];
            int isPresent = window.isValid() ? 1:0;s.save(&isPresent,"isWindowPresent");
            if (isPresent)  {
                float tmp = window.size<0?window.size:(window.size/(pos<ImGui::PanelManager::TOP?displaySize.x:displaySize.y));
                s.save(&tmp,"sizeFactor");
                tmp = window.sizeHoverMode<0?window.sizeHoverMode:(window.sizeHoverMode/(pos<ImGui::PanelManager::TOP?displaySize.x:displaySize.y));
                s.save(&tmp,"hoverModeSizeFactor");
                int wf = (int) window.extraWindowFlags;s.save(ImGui::FT_ENUM,&wf,"extraWindowFlags");
            }
            int down = (button.isDown && button.isToggleButton) ? 1 : 0;
            s.save(&down,"toggleButtonIsPressed");
        }
    }
    static const char* endText="End";s.saveTextLines(endText,"endPanelManager");
    return true;
}
#endif //NO_IMGUIHELPER_SERIALIZATION_SAVE
#ifndef NO_IMGUIHELPER_SERIALIZATION_LOAD
#include "../imguihelper/imguihelper.h"
struct ImGuiPanelManagerParserStruct {
    ImVec2 displaySize;
    ImGui::PanelManager* mgr;
    ImGui::PanelManager::Pane* curPane;
    ImGui::PanelManager::Pane::AssociatedWindow* curWindow;
    int numPanes,curPaneNum;
    int numButtons,curButtonNum;
};
static bool ImGuiPanelManagerParser(ImGuiHelper::FieldType ft,int /*numArrayElements*/,void* pValue,const char* name,void* userPtr)    {

    ImGuiPanelManagerParserStruct& ps = *((ImGuiPanelManagerParserStruct*) userPtr);
    ImGui::PanelManager& mgr = *ps.mgr;

    switch (ft) {
    case ImGui::FT_FLOAT:   {
        if (strcmp(name,"sizeFactor")==0)			{
            if (ps.curWindow && ps.curPane) {
                ps.curWindow->size = *((float*)pValue);
                if(ps.curWindow->size>0) ps.curWindow->size*=(ps.curPane->pos<ImGui::PanelManager::TOP?ps.displaySize.x:ps.displaySize.y);
            }
            break;
        }
        else if (strcmp(name,"hoverModeSizeFactor")==0)	{
            if (ps.curWindow && ps.curPane) {
                ps.curWindow->sizeHoverMode = *((float*)pValue);
                if (ps.curWindow->sizeHoverMode>0) ps.curWindow->sizeHoverMode*=(ps.curPane->pos<ImGui::PanelManager::TOP?ps.displaySize.x:ps.displaySize.y);
            }
            break;
        }
    }
        break;
    case ImGui::FT_ENUM:    {
        if (strcmp(name,"pos")==0)	{
            ++ps.curPaneNum;ps.curPane = NULL;ps.curButtonNum=-1;ps.numButtons=0;ps.curWindow=NULL;
            if (ps.curPaneNum<ps.numPanes && ps.curPaneNum<(int)mgr.getNumPanes() && mgr.getPaneFromIndex(ps.curPaneNum)->pos==*((int*)pValue)) ps.curPane=mgr.getPaneFromIndex(ps.curPaneNum);
            //else fprintf(stderr,"Invalid pane\n");
            break;
        }
        else if (strcmp(name,"extraWindowFlags")==0)	{
            if (ps.curWindow) {
                ps.curWindow->extraWindowFlags=*((int*)pValue);
            }
            break;
        }
        else if (strcmp(name,"managerExtraWindowFlags")==0)	{
            ImGuiWindowFlags flags = *((int*)pValue);
            //mgr.dockedWindowsExtraFlags = flags;
            //mgr.setDockedWindowsBorder(flags&ImGuiWindowFlags_ShowBorders);
            mgr.setDockedWindowsNoTitleBar(flags&ImGuiWindowFlags_NoTitleBar);
            break;
        }
    }
        break;
    case ImGui::FT_INT: {
        if (strcmp(name,"numPanes")==0)	{
            ps.numPanes=*((int*)pValue);ps.curPaneNum = -1;ps.curPane=NULL;
            ps.numButtons = 0;ps.curButtonNum=-1;ps.curWindow=NULL;
            break;
        }
        else if (strcmp(name,"selectedButtonIndex")==0)	{
            if (ps.curPane) {
                ps.curPane->bar.setSelectedButtonIndex(*((int*)pValue));
            }
            break;
        }
        else if (strcmp(name,"numPaneButtons")==0)	{
            ps.numButtons=*((int*)pValue);ps.curButtonNum=-1;ps.curWindow=NULL;
            if (ps.curPane && (int)ps.curPane->bar.getNumButtons()!=ps.numButtons)	{
                ps.curPane = NULL;  // Skip if number does not match
            }
            if (ps.curPane) {
                IM_ASSERT((int)ps.curPane->windows.size()==(int)ps.curPane->bar.getNumButtons());
            }
            break;
        }
        if (strcmp(name,"isWindowPresent")==0)		{
            ++ps.curButtonNum;
            if (ps.curPane && ps.curButtonNum<ps.numButtons && ps.curButtonNum<(int)ps.curPane->bar.getNumButtons())  ps.curWindow=&ps.curPane->windows[ps.curButtonNum];
            else {ps.curPane = NULL;ps.curWindow = NULL;}
            //fprintf(stderr,"pane:%d/%d == %s button:%d/%d window == %s\n",ps.curPaneNum,ps.numPanes,ps.curPane?"OK":"NULL",ps.curButtonNum,ps.numButtons,ps.curWindow? (ps.curWindow->windowName?ps.curWindow->windowName:"OK"):"NULL");
            //bool isWindowPresent = *((bool*)pValue) ? true : false;
            break;
        }
        else if (strcmp(name,"toggleButtonIsPressed")==0)	{
            bool down = *((int*)pValue) ? true : false;
            if (ps.curPane && ps.curButtonNum<ps.numButtons && (int)ps.curPane->bar.getNumButtons()>ps.curButtonNum)	{
                ImGui::Toolbar::Button& btn = *(ps.curPane->bar.getButton(ps.curButtonNum));
                if (btn.isToggleButton) btn.isDown = down;
            }
            break;
        }
    }
        break;
    case ImGui::FT_TEXTLINE:
        if (strcmp(name,"endPanelManager")==0) return true;
        break;
    default:
        break;
    }
    return false;
}
bool ImGui::PanelManager::Load(PanelManager &mgr,ImGuiHelper::Deserializer& d, const char ** pOptionalBufferStart)  {
    if (!d.isValid()) return false;
    ImGuiPanelManagerParserStruct pmps;
    pmps.mgr = &mgr;pmps.numButtons=pmps.curButtonNum=pmps.numPanes=pmps.curPaneNum=0;pmps.curPane=NULL;pmps.curWindow=NULL;
    pmps.displaySize = ImGui::GetIO().DisplaySize;
    IM_ASSERT(pmps.displaySize.x>=0 && pmps.displaySize.y>=0);

    const char* amount = pOptionalBufferStart ? (*pOptionalBufferStart) : 0;
    amount = d.parse(ImGuiPanelManagerParser,(void*)&pmps,amount);
    if (pOptionalBufferStart) *pOptionalBufferStart=amount;

    mgr.recalculatePositionAndSizes();
    mgr.closeHoverWindow();

    return true;
}
#endif //NO_IMGUIHELPER_SERIALIZATION_LOAD
#endif //NO_IMGUIHELPER_SERIALIZATION
