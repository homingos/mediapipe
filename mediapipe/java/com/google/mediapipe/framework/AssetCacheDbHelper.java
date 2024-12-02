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

package com.google.mediapipe.framework;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.provider.BaseColumns;
import com.google.common.flogger.FluentLogger;
import java.io.File;

/**
 * Database to keep the cached version of asset valid.
 */
public class AssetCacheDbHelper extends SQLiteOpenHelper {
    private static final FluentLogger logger = FluentLogger.forEnclosingClass();

    public static final int DATABASE_VERSION = 2;
    public static final String DATABASE_NAME = "mediapipe.db";

    private static final String INT_TYPE = " INTEGER";
    private static final String TEXT_TYPE = " TEXT";
    private static final String TEXT_UNIQUE_TYPE = " TEXT NOT NULL UNIQUE";
    private static final String COMMA_SEP = ",";
    private static final String SQL_CREATE_TABLE =
        "CREATE TABLE " + AssetCacheEntry.TABLE_NAME + " ("
        + AssetCacheEntry._ID + " INTEGER PRIMARY KEY,"
        + AssetCacheEntry.COLUMN_NAME_ASSET + TEXT_UNIQUE_TYPE + COMMA_SEP
        + AssetCacheEntry.COLUMN_NAME_CACHE_PATH + TEXT_TYPE + COMMA_SEP
        + AssetCacheEntry.COLUMN_NAME_VERSION + INT_TYPE + " )";

    private static final String SQL_DELETE_TABLE =
        "DROP TABLE IF EXISTS " + AssetCacheEntry.TABLE_NAME;

    /**
     * The columns in the AssetVersion table.
     */
    public abstract static class AssetCacheEntry implements BaseColumns {
      public static final String TABLE_NAME = "AssetVersion";
      public static final String COLUMN_NAME_ASSET = "asset";
      public static final String COLUMN_NAME_CACHE_PATH = "cache_path";
      public static final String COLUMN_NAME_VERSION = "version";
    }

    public AssetCacheDbHelper(Context context) {
      super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    /**
     * Check if the cached version is current in database.
     *
     * @return true if the asset is cached and the app is not upgraded. Otherwise return false.
     */
    public boolean checkVersion(String assetPath, int currentAppVersion) {
      SQLiteDatabase db = getReadableDatabase();
      String selection = AssetCacheEntry.COLUMN_NAME_ASSET + " = ?";
      String[] projection = {AssetCacheEntry.COLUMN_NAME_VERSION};
      String[] selectionArgs = {assetPath};

      Cursor cursor = queryAssetCacheTable(db, projection, selection, selectionArgs);

      if (cursor.getCount() == 0) {
        return false;
      }

      cursor.moveToFirst();
      int cachedVersion = cursor.getInt(
          cursor.getColumnIndexOrThrow(AssetCacheEntry.COLUMN_NAME_VERSION));
      cursor.close();
      return cachedVersion == currentAppVersion;
    }

    /**
     * Remove all entries in the version table that don't have the correct version.
     *
     * <p>Invalidates all cached asset contents that doesn't have the specified version.
     */
    public void invalidateCache(int currentAppVersion) {
      SQLiteDatabase db = getWritableDatabase();
      String selection = AssetCacheEntry.COLUMN_NAME_VERSION + " != ?";
      String[] selectionArgs = {Integer.toString(currentAppVersion)};
      // Remve the cached files.
      removeCachedFiles(db, selection, selectionArgs);
      // Remve the rows in the table.
      db.delete(AssetCacheEntry.TABLE_NAME, selection, selectionArgs);
    }

    /**
     * Insert the cached version of the asset into the database.
     */
    public void insertAsset(String asset, String cachePath, int appVersion) {
      SQLiteDatabase db = getWritableDatabase();
      // Remove the old cached file first if they are different from the new cachePath.
      String selection = AssetCacheEntry.COLUMN_NAME_ASSET + " = ? and "
          + AssetCacheEntry.COLUMN_NAME_CACHE_PATH + " != ?";
      String[] selectionArgs = {asset, cachePath};
      removeCachedFiles(db, selection, selectionArgs);

      ContentValues values = new ContentValues();
      values.put(AssetCacheEntry.COLUMN_NAME_ASSET, asset);
      values.put(AssetCacheEntry.COLUMN_NAME_CACHE_PATH, cachePath);
      values.put(AssetCacheEntry.COLUMN_NAME_VERSION, appVersion);
      long newRowId = db.insertWithOnConflict(
         AssetCacheEntry.TABLE_NAME,
         null,
         values,
         SQLiteDatabase.CONFLICT_REPLACE);
      // According to documentation, -1 means any error.
      if (newRowId == -1) {
      throw new RuntimeException("Can't insert entry into the mediapipe db.");
      }
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
      db.execSQL(SQL_CREATE_TABLE);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
      // Since version 1 doesn't have the path in the table, just upgrade the table.
      db.execSQL(SQL_DELETE_TABLE);
      onCreate(db);
    }

    @Override
    public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
      onUpgrade(db, oldVersion, newVersion);
    }

    private Cursor queryAssetCacheTable(
        SQLiteDatabase db, String[] projection, String selection, String[] selectionArgs) {
      return db.query(
          AssetCacheEntry.TABLE_NAME,  // The table to query
          projection,                  // The columns to return
          selection,                   // The columns for the WHERE clause
          selectionArgs,               // The values for the WHERE clause
          null,                        // don't group the rows
          null,                        // don't filter by row groups
          null                         // The sort order
      );
    }

    private void removeCachedFiles(SQLiteDatabase db, String selection, String[] selectionArgs) {
      String[] projection = {AssetCacheEntry.COLUMN_NAME_CACHE_PATH};
      Cursor cursor = queryAssetCacheTable(db, projection, selection, selectionArgs);
      if (cursor.moveToFirst()) {
        do {
          String cachedPath = cursor.getString(
              cursor.getColumnIndexOrThrow(AssetCacheEntry.COLUMN_NAME_CACHE_PATH));
          File file = new File(cachedPath);
          if (file.exists()) {
            if (!file.delete()) {
              logger.atWarning().log("Stale cached file: %s can't be deleted.", cachedPath);
            }
          }
        } while (cursor.moveToNext());
      }
      cursor.close();
    }
}
