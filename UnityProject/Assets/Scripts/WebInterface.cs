using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class WebInterface
{
    [DllImport ("WebInterface")] public static extern void wiStartServer();
    [DllImport ("WebInterface")] public static extern void wiStopServer();
    [DllImport ("WebInterface")] public static extern void wiSetViewProjectionMatrix(Matrix4x4 view, Matrix4x4 proj);
    [DllImport ("WebInterface")] public static extern void wiAddGameObject(int id);
    [DllImport ("WebInterface")] public static extern void wiDeleteGameObject(int id);
}
