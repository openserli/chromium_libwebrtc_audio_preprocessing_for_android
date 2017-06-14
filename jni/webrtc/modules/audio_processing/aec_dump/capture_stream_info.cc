/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "webrtc/modules/audio_processing/aec_dump/capture_stream_info.h"

namespace webrtc {
CaptureStreamInfo::CaptureStreamInfo(std::unique_ptr<WriteToFileTask> task)
    : task_(std::move(task)) {
  RTC_DCHECK(task_);
  task_->GetEvent()->set_type(audioproc::Event::STREAM);
}

CaptureStreamInfo::~CaptureStreamInfo() = default;

void CaptureStreamInfo::AddInput(const FloatAudioFrame& src) {
  RTC_DCHECK(task_);
  auto* stream = task_->GetEvent()->mutable_stream();

  for (size_t i = 0; i < src.num_channels(); ++i) {
    const auto& channel_view = src.channel(i);
    stream->add_input_channel(channel_view.begin(),
                              sizeof(float) * channel_view.size());
  }
}

void CaptureStreamInfo::AddOutput(const FloatAudioFrame& src) {
  RTC_DCHECK(task_);
  auto* stream = task_->GetEvent()->mutable_stream();

  for (size_t i = 0; i < src.num_channels(); ++i) {
    const auto& channel_view = src.channel(i);
    stream->add_output_channel(channel_view.begin(),
                               sizeof(float) * channel_view.size());
  }
}

void CaptureStreamInfo::AddInput(const AudioFrame& frame) {
  RTC_DCHECK(task_);
  auto* stream = task_->GetEvent()->mutable_stream();
  const size_t data_size =
      sizeof(int16_t) * frame.samples_per_channel_ * frame.num_channels_;
  stream->set_input_data(frame.data_, data_size);
}

void CaptureStreamInfo::AddOutput(const AudioFrame& frame) {
  RTC_DCHECK(task_);
  auto* stream = task_->GetEvent()->mutable_stream();
  const size_t data_size =
      sizeof(int16_t) * frame.samples_per_channel_ * frame.num_channels_;
  stream->set_output_data(frame.data_, data_size);
}

void CaptureStreamInfo::AddAudioProcessingState(
    const AecDump::AudioProcessingState& state) {
  RTC_DCHECK(task_);
  auto* stream = task_->GetEvent()->mutable_stream();
  stream->set_delay(state.delay);
  stream->set_drift(state.drift);
  stream->set_level(state.level);
  stream->set_keypress(state.keypress);
}
}  // namespace webrtc
