package com.google.mediapipe.apps.basic;

public class ShaderManager {

    //#region Primary Shaders
    public static final String vertexShaderCode
    = "attribute vec4 vPosition;\n"
    + "attribute vec2 aTexCoord;\n"
    + "uniform mat4 uMVPMatrix;\n"
    + "varying vec2 vTexCoord;\n"
    + "void main() {\n"
    + "  gl_Position = uMVPMatrix * vPosition;\n"
    + "  vTexCoord = aTexCoord;\n"
    + "}\n";

    public static final String fragmentShaderCode
        = "#extension GL_OES_EGL_image_external : require\n"
        + "precision mediump float;\n"
        + "uniform samplerExternalOES uTexture;\n"
        + "varying vec2 vTexCoord;\n"
        + "void main() {\n"
        + "  gl_FragColor = texture2D(uTexture, vTexCoord);\n"
        // + "  gl_FragColor = vec4(1.0f,0.6f,0.2f,1.0f);\n" // Orange for debugging
        + "}\n";

    public static final String bgVertexShaderCode
        = "attribute vec4 vPosition;\n"
        + "attribute vec2 aTexCoord;\n"
        + "uniform mat4 uMVPMatrix;\n"
        + "varying vec2 vTexCoord;\n"
        + "void main() {\n"
        + "  gl_Position = uMVPMatrix * vPosition;\n"
        + "  vTexCoord = aTexCoord;\n"
        + "}\n";

    public static final String bgFragmentShaderCode
        = "#extension GL_OES_EGL_image_external : require\n"
        + "precision mediump float;\n"
        + "uniform samplerExternalOES bgTexture;\n"
        + "varying vec2 vTexCoord;\n"
        + "void main() {\n"
        + "  gl_FragColor = texture2D(bgTexture, vTexCoord);\n"
        + "}\n";
    //#endregion
    
}
