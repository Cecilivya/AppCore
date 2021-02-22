#pragma once
#include <AppCore/Window.h>
#include <Ultralight/Listener.h>
#include "RefCountedImpl.h"
#include "OverlayManager.h"
#import "Cocoa/Cocoa.h"
#import "ViewController.h"
#import "WindowDelegate.h"
#import <MetalKit/MetalKit.h>
#import <QuartzCore/QuartzCore.h>

namespace ultralight {

class WindowMac : public Window,
                  public RefCountedImpl<WindowMac>,
                  public OverlayManager {
public:
  virtual void set_listener(WindowListener* listener) override { listener_ = listener; }

  virtual WindowListener* listener() override { return listener_; }

  virtual uint32_t width() const override;

  virtual uint32_t height() const override;

  virtual bool is_fullscreen() const override { return is_fullscreen_; }
                    
  virtual bool is_accelerated() const override { return is_accelerated_; }
                
  virtual uint32_t render_buffer_id() const override { return render_buffer_id_; }

  virtual double scale() const override;

  virtual void SetTitle(const char* title) override;

  virtual void SetCursor(ultralight::Cursor cursor) override;

  virtual void Close() override;

  virtual int DeviceToPixels(int val) const override {
    return (int)round(val * scale());
  }

  virtual int PixelsToDevice(int val) const override {
    return (int)round(val / scale());
  }

  virtual void* native_handle() const override;

  virtual OverlayManager* overlay_manager() const override { return const_cast<WindowMac*>(this); }

  REF_COUNTED_IMPL(WindowMac);

  void set_app_listener(WindowListener* listener) { app_listener_ = listener; }
                      
  void OnClose();
  
  void OnResize(uint32_t width, uint32_t height);
                      
  void SetNeedsDisplay();
  
  void OnPaint(CAMetalLayer* layer);
                      
  CAMetalLayer* layer();
  
  id <CAMetalDrawable> current_drawable() { return current_drawable_; }

protected:
  WindowMac(Monitor* monitor, uint32_t width, uint32_t height,
    bool fullscreen, unsigned int window_flags);

  virtual ~WindowMac();

  friend class Window;

  DISALLOW_COPY_AND_ASSIGN(WindowMac);

  NSWindow* window_;
  ViewController* controller_;
  WindowDelegate* delegate_;
  WindowListener* listener_ = nullptr;
  WindowListener* app_listener_ = nullptr;
  Monitor* monitor_;
  bool is_fullscreen_;
  bool is_accelerated_;
  uint32_t render_buffer_id_;
  id <CAMetalDrawable> current_drawable_;
};

}  // namespace ultralight

