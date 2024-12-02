package com.google.mediapipe.components;

import android.content.Context;

public class CloudScanConfig {
    private final Context context;
    private final String binaryGraphName;
    private final String inputStreamName;
    private final boolean flipFramesVertically;
    private final int numBuffers;
    private final String serverEndpoint;
    
    private CloudScanConfig(Builder builder) {
        this.context = builder.context;
        this.binaryGraphName = builder.binaryGraphName;
        this.inputStreamName = builder.inputStreamName;
        this.flipFramesVertically = builder.flipFramesVertically;
        this.numBuffers = builder.numBuffers;
        this.serverEndpoint = builder.serverEndpoint;
    }

    // Getters
    public Context getContext() { return context; }
    public String getBinaryGraphName() { return binaryGraphName; }
    public String getInputStreamName() { return inputStreamName; }
    public boolean isFlipFramesVertically() { return flipFramesVertically; }
    public int getNumBuffers() { return numBuffers; }
    public String getServerEndpoint() { return serverEndpoint; }
    
    public static class Builder {
        private final Context context;
        private String binaryGraphName;
        private String inputStreamName;
        private boolean flipFramesVertically = true;
        private int numBuffers = 2;
        private String serverEndpoint;
        
        public Builder(Context context) {
            this.context = context;
        }
        
        public Builder setBinaryGraphName(String name) {
            this.binaryGraphName = name;
            return this;
        }
        
        public Builder setInputStreamName(String name) {
            this.inputStreamName = name;
            return this;
        }
        
        public Builder setFlipFramesVertically(boolean flip) {
            this.flipFramesVertically = flip;
            return this;
        }
        
        public Builder setNumBuffers(int num) {
            this.numBuffers = num;
            return this;
        }
        
        public Builder setServerEndpoint(String endpoint) {
            this.serverEndpoint = endpoint;
            return this;
        }
        
        public CloudScanConfig build() {
            if (binaryGraphName == null) {
                throw new IllegalStateException("Binary graph name is required");
            }
            if (inputStreamName == null) {
                throw new IllegalStateException("Input stream name is required");  
            }
            if (serverEndpoint == null) {
                throw new IllegalStateException("Server endpoint is required");
            }
            return new CloudScanConfig(this);
        }
    }
}