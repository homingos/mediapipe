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
r"""Code to clone the github repository, download the checkpoint and generate the frozen graph.

The frozen VGGish checkpoint will be saved to `/tmp/mediapipe/vggish_new.pb`.
"""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import sys

from absl import app
import tensorflow.compat.v1 as tf
from tensorflow.python.tools import freeze_graph

BASE_DIR = '/tmp/mediapipe/'


def create_vggish_frozen_graph():
  """Create the VGGish frozen graph."""
  os.system('git clone https://github.com/tensorflow/models.git')
  sys.path.append('models/research/audioset/vggish/')

  import vggish_slim
  os.system('curl -O https://storage.googleapis.com/audioset/vggish_model.ckpt')
  ckpt_path = 'vggish_model.ckpt'

  with tf.Graph().as_default(), tf.Session() as sess:
    vggish_slim.define_vggish_slim(training=False)
    vggish_slim.load_vggish_slim_checkpoint(sess, ckpt_path)

    saver = tf.train.Saver(tf.all_variables())

    freeze_graph.freeze_graph_with_def_protos(
        sess.graph_def,
        saver.as_saver_def(),
        ckpt_path,
        'vggish/fc2/BiasAdd',
        restore_op_name=None,
        filename_tensor_name=None,
        output_graph='/tmp/mediapipe/vggish_new.pb',
        clear_devices=True,
        initializer_nodes=None)
  os.system('rm -rf models/')
  os.system('rm %s' % ckpt_path)


def main(argv):
  if len(argv) > 1:
    raise app.UsageError('Too many command-line arguments.')

  create_vggish_frozen_graph()


if __name__ == '__main__':
  app.run(main)
