# Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
#
# PROPRIETARY AND CONFIDENTIAL
# 
# This software and its documentation are the confidential and proprietary
# information of Flamingos Technologies Inc. ("Confidential Information").
# You shall not disclose such Confidential Information and shall use it
# only in accordance with the terms of the license agreement you entered
# into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
# or use of this software in source or binary forms is strictly prohibited.

"""Generate a MediaSequence metadata for MediaPipe input."""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import sys

from absl import app
from absl import flags
import six
import tensorflow.compat.v1 as tf
from mediapipe.util.sequence import media_sequence as ms

FLAGS = flags.FLAGS
SECONDS_TO_MICROSECONDS = 1000000


def bytes23(string):
  """Creates a bytes string in either Python 2 or 3."""
  if sys.version_info >= (3, 0):
    return bytes(string, 'utf8')
  else:
    return bytes(string)


def main(argv):
  if len(argv) > 3:
    raise app.UsageError('Too many command-line arguments.')

  if not flags.FLAGS.path_to_input_video:
    raise ValueError('You must specify the path to the input video.')
  if not flags.FLAGS.clip_end_time_sec:
    raise ValueError('You must specify the clip end timestamp in seconds.')
  if flags.FLAGS.clip_start_time_sec >= flags.FLAGS.clip_end_time_sec:
    raise ValueError(
        'The clip start time must be greater than the clip end time.')
  metadata = tf.train.SequenceExample()
  ms.set_clip_data_path(bytes23(flags.FLAGS.path_to_input_video), metadata)
  ms.set_clip_start_timestamp(
      flags.FLAGS.clip_start_time_sec * SECONDS_TO_MICROSECONDS, metadata)
  ms.set_clip_end_timestamp(
      flags.FLAGS.clip_end_time_sec * SECONDS_TO_MICROSECONDS, metadata)
  with open('/tmp/mediapipe/metadata.pb', 'wb') as writer:
    writer.write(six.ensure_binary(metadata.SerializeToString()))


if __name__ == '__main__':
  flags.DEFINE_string('path_to_input_video', '', 'Path to the input video.')
  flags.DEFINE_integer('clip_start_time_sec', 0,
                       'Clip start timestamp in seconds')
  flags.DEFINE_integer('clip_end_time_sec', 10, 'Clip end timestamp in seconds')
  app.run(main)
