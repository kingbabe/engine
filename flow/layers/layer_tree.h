// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_FLOW_LAYERS_LAYER_TREE_H_
#define FLUTTER_FLOW_LAYERS_LAYER_TREE_H_

#include <stdint.h>

#include <memory>

#include "flutter/flow/compositor_context.h"
#include "flutter/flow/layers/layer.h"
#include "lib/fxl/macros.h"
#include "lib/fxl/time/time_delta.h"
#if defined(OS_FUCHSIA)
#include <fuchsia/cpp/ui.h>
#endif
#include "third_party/skia/include/core/SkSize.h"

namespace flow {

class LayerTree {
 public:
  LayerTree();

  ~LayerTree();

  // Raster includes both Preroll and Paint.
  void Raster(CompositorContext::ScopedFrame& frame,
#if defined(OS_FUCHSIA)
              gfx::Metrics* metrics,
#endif
              bool ignore_raster_cache = false);

  void Preroll(CompositorContext::ScopedFrame& frame,
#if defined(OS_FUCHSIA)
               gfx::Metrics* metrics,
#endif
               bool ignore_raster_cache = false);

#if defined(OS_FUCHSIA)
  void set_device_pixel_ratio(float device_pixel_ratio) {
    device_pixel_ratio_ = device_pixel_ratio;
  }

  void UpdateScene(SceneUpdateContext& context,
                   scenic_lib::ContainerNode& container);
#endif

  void Paint(CompositorContext::ScopedFrame& frame) const;

  Layer* root_layer() const { return root_layer_.get(); }

  void set_root_layer(std::unique_ptr<Layer> root_layer) {
    root_layer_ = std::move(root_layer);
  }

  const SkISize& frame_size() const { return frame_size_; }

  void set_frame_size(const SkISize& frame_size) { frame_size_ = frame_size; }

  void set_construction_time(const fxl::TimeDelta& delta) {
    construction_time_ = delta;
  }

  const fxl::TimeDelta& construction_time() const { return construction_time_; }

  // The number of frame intervals missed after which the compositor must
  // trace the rasterized picture to a trace file. Specify 0 to disable all
  // tracing
  void set_rasterizer_tracing_threshold(uint32_t interval) {
    rasterizer_tracing_threshold_ = interval;
  }

  uint32_t rasterizer_tracing_threshold() const {
    return rasterizer_tracing_threshold_;
  }

  void set_checkerboard_raster_cache_images(bool checkerboard) {
    checkerboard_raster_cache_images_ = checkerboard;
  }

  void set_checkerboard_offscreen_layers(bool checkerboard) {
    checkerboard_offscreen_layers_ = checkerboard;
  }

 private:
  SkISize frame_size_;  // Physical pixels.
  std::unique_ptr<Layer> root_layer_;
  fxl::TimeDelta construction_time_;
  uint32_t rasterizer_tracing_threshold_;
  bool checkerboard_raster_cache_images_;
  bool checkerboard_offscreen_layers_;

#if defined(OS_FUCHSIA)
  float device_pixel_ratio_ = 1.f;
#endif

  FXL_DISALLOW_COPY_AND_ASSIGN(LayerTree);
};

}  // namespace flow

#endif  // FLUTTER_FLOW_LAYERS_LAYER_TREE_H_
