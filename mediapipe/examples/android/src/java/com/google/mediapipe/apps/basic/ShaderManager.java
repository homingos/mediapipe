package com.google.mediapipe.apps.basic;

public class ShaderManager {

    //#region Primary Shaders
    

    public static final String VERTEX_SHADER_CODE =
        "attribute vec4 vPosition;" +
        "attribute vec2 a_TexCoordinate;" +
        "varying vec2 v_TexCoordinate;" +
        "void main() {" +
        "  gl_Position = vPosition;" +
        "  v_TexCoordinate = a_TexCoordinate;" +
        "}";

    public static final String FRAGMENT_SHADER_CODE =
        "precision mediump float;" +
        "uniform sampler2D u_Texture;" +
        "varying vec2 v_TexCoordinate;" +
        "void main() {" +
        "  gl_FragColor = texture2D(u_Texture, v_TexCoordinate);" +
        "}";
    //#endregion
    
}
