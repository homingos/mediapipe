// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_PACKET_CREATOR_JNI_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_PACKET_CREATOR_JNI_H_

#include <jni.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define PACKET_CREATOR_METHOD(METHOD_NAME) \
    Java_com_google_mediapipe_framework_PacketCreator_##METHOD_NAME

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateReferencePacket)(
        JNIEnv *env, jobject thiz, jlong context, jlong packet);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateRgbImage)(
        JNIEnv *env, jobject thiz, jlong context, jobject byte_buffer, jint width,
        jint height);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateFloatImageFrame)(
        JNIEnv *env, jobject thiz, jlong context, jobject byte_buffer, jint width,
        jint height);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateRgbaImageFrame)(
        JNIEnv *env, jobject thiz, jlong context, jobject byte_buffer, jint width,
        jint height);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateRgbImageFromRgba)(
        JNIEnv *env, jobject thiz, jlong context, jobject byte_buffer, jint width,
        jint height);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateGrayscaleImage)(
        JNIEnv *env, jobject thiz, jlong context, jobject byte_buffer, jint width,
        jint height);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateAudioPacketDirect)(
        JNIEnv *env, jobject thiz, jlong context, jobject data, jint num_channels,
        jint num_samples);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateAudioPacket)(
        JNIEnv *env, jobject thiz, jlong context, jbyteArray data, jint offset,
        jint num_channels, jint num_samples);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateInt16)(JNIEnv *env,
                                                                     jobject thiz,
                                                                     jlong context,
                                                                     jshort value);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateInt32)(JNIEnv *env,
                                                                     jobject thiz,
                                                                     jlong context,
                                                                     jint value);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateInt64)(JNIEnv *env,
                                                                     jobject thiz,
                                                                     jlong context,
                                                                     jlong value);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateFloat32)(
        JNIEnv *env, jobject thiz, jlong context, jfloat value);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateFloat64)(
        JNIEnv *env, jobject thiz, jlong context, jdouble value);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateBool)(JNIEnv *env,
                                                                    jobject thiz,
                                                                    jlong context,
                                                                    jboolean value);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateString)(
        JNIEnv *env, jobject thiz, jlong context, jstring value);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateStringArray)(
        JNIEnv *env, jobject thiz, jlong context, jobjectArray string_array);
    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateVideoHeader)(
        JNIEnv *env, jobject thiz, jlong context, jint width, jint height);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateTimeSeriesHeader)(
        JNIEnv *env, jobject thiz, jlong context, jint num_channels,
        jdouble sample_rate);

    // Creates a MediaPipe::Matrix packet using the float array data.
    // The data must in column major order.
    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateMatrix)(
        JNIEnv *env, jobject thiz, jlong context, jint rows, jint cols,
        jfloatArray data);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateCpuImage)(
        JNIEnv *env, jobject thiz, jlong context, jobject byte_buffer, jint width,
        jint height, jint width_step, jint num_channels);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateGpuImage)(
        JNIEnv *env, jobject thiz, jlong context, jint name, jint width,
        jint height, jobject texture_release_callback);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateGpuBuffer)(
        JNIEnv *env, jobject thiz, jlong context, jint name, jint width,
        jint height, jobject texture_release_callback);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateFloat32Array)(
        JNIEnv *env, jobject thiz, jlong context, jfloatArray data);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateFloat32Vector)(
        JNIEnv *env, jobject thiz, jlong context, jfloatArray data);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateInt32Array)(
        JNIEnv *env, jobject thiz, jlong context, jintArray data);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateInt32Pair)(
        JNIEnv *env, jobject thiz, jlong context, jint first, jint second);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateStringFromByteArray)(
        JNIEnv *env, jobject thiz, jlong context, jbyteArray data);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateCalculatorOptions)(
        JNIEnv *env, jobject thiz, jlong context, jbyteArray data);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateProto)(JNIEnv *env,
                                                                     jobject thiz,
                                                                     jlong context,
                                                                     jobject data);

    JNIEXPORT jlong JNICALL PACKET_CREATOR_METHOD(nativeCreateCameraIntrinsics)(
        JNIEnv *env, jobject thiz, jlong context, jfloat fx, jfloat fy, jfloat cx,
        jfloat cy, jfloat width, jfloat height);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_PACKET_CREATOR_JNI_H_
