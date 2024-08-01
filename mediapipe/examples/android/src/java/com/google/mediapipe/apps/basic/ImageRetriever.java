package com.google.mediapipe.apps.basic;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Log;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class ImageRetriever extends AsyncTask<String, Void, Bitmap> {

    private static final String TAG = "ImageRetriever";
    private ImageCallback callback;

    public ImageRetriever(ImageCallback callback) {
        this.callback = callback;
    }

    @Override
    protected Bitmap doInBackground(String... imageUrls) {
        if (imageUrls.length == 0) {
            Log.e(TAG, "No image URL provided");
            return null;
        }
        String imageUrl = imageUrls[0];
        try {
            URL url = new URL(imageUrl);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            connection.setDoInput(true);
            connection.connect();

            InputStream inputStream = connection.getInputStream();
            Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
            return bitmap;
        } catch (Exception e) {
            Log.e(TAG, "Error retrieving image: " + e.getMessage());
            return null;
        }
    }

    @Override
    protected void onPostExecute(Bitmap bitmap) {
        if (callback != null) {
            callback.onImageRetrieved(bitmap);
        }
    }

    public interface ImageCallback {

        void onImageRetrieved(Bitmap bitmap);
    }
}
