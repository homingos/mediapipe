package com.google.mediapipe.apps.basic;

public class ShaderManager {

    //#region Primary Shaders
    public static final String vertexShaderCode
    = "attribute lowp vec4 vPosition;\n" +      // Reduced precision for position
        "attribute lowp vec2 aTexCoord;\n" +      // Reduced precision for texture coordinates
        "uniform mediump mat4 uMVPMatrix;\n" +    // Keep medium precision for matrix
        "varying lowp vec2 vTexCoord;\n" +        // Reduced precision for output coordinates
        "void main() {\n" +
        "  gl_Position = uMVPMatrix * vPosition;\n" +
        "  vTexCoord = aTexCoord;\n" +
        "}\n";

    public static final String fragmentShaderCode
        =  "#extension GL_OES_EGL_image_external : require\n" +
        "precision lowp float;\n" +               // Use lowp for better performance
        "uniform samplerExternalOES uTexture;\n" +
        "varying lowp vec2 vTexCoord;\n" +
        "void main() {\n" +
        "  gl_FragColor = texture2D(uTexture, vTexCoord);\n" +
        "}\n";

    public static final String bgVertexShaderCode
        = "attribute mediump vec4 vPosition;\n"
        + "attribute mediump vec2 aTexCoord;\n"
        + "uniform mediump mat4 uMVPMatrix;\n"
        + "varying mediump vec2 vTexCoord;\n"
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
