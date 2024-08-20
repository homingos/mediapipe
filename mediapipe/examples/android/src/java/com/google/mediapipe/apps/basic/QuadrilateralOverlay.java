package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;
import android.util.Log;

public class QuadrilateralOverlay extends View {

    private float[] points;
    private Paint paint;

    public QuadrilateralOverlay(Context context) {
        super(context);
        paint = new Paint();
        paint.setColor(Color.GREEN);
        paint.setStrokeWidth(5);
        paint.setStyle(Paint.Style.STROKE);
    }

    public void setPoints(float[] points) {
        this.points = points;
        invalidate(); // Redraw the view when points are updated
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if (points != null && points.length == 8) {
            // Convert normalized coordinates to screen coordinates
            float canvasWidth = canvas.getWidth();
            float canvasHeight = canvas.getHeight();
            float[] screenPoints = new float[8];
            for (int i = 0; i < 8; i += 2) {
                screenPoints[i] = points[i] * canvasWidth;
                screenPoints[i + 1] = points[i + 1] * canvasHeight;
            }

            // Draw the quadrilateral
            Log.e("onDraw", "Drawing");
            canvas.drawLine(screenPoints[0], screenPoints[1], screenPoints[2], screenPoints[3], paint);
            canvas.drawLine(screenPoints[2], screenPoints[3], screenPoints[4], screenPoints[5], paint);
            canvas.drawLine(screenPoints[4], screenPoints[5], screenPoints[6], screenPoints[7], paint);
            canvas.drawLine(screenPoints[6], screenPoints[7], screenPoints[0], screenPoints[1], paint);
        }
    }
}
