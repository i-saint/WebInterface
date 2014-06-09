using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;


public class wi
{
    public struct wiEntityData
    {
        public Matrix4x4 transform;
        public Vector4 size;
        public Vector4 color;
        public int id;
        public int pad1, pad2, pad3;
    }

    public unsafe struct wiKeyValue
    {
        public char* name;
        public char* value;
    }
    public unsafe delegate void wiCallback(int num, wiKeyValue* kvp);

    [DllImport ("WebInterface")] public static extern void wiStartServer();
    [DllImport ("WebInterface")] public static extern void wiStopServer();
    [DllImport ("WebInterface")] public static extern void wiUpdate();

    [DllImport ("WebInterface")] public static extern void wiSetViewProjectionMatrix(Matrix4x4 view, Matrix4x4 proj);
    [DllImport ("WebInterface")] public static extern void wiSetConnectCallback(wiCallback cb);
    [DllImport ("WebInterface")] public static extern void wiSetDisconnectCallback(wiCallback cb);
    [DllImport ("WebInterface")] public static extern void wiSetSelectCallback(wiCallback cb);
    [DllImport ("WebInterface")] public static extern void wiSetDisselectCallback(wiCallback cb);
    [DllImport ("WebInterface")] public static extern void wiSetActionCallback(wiCallback cb);

    [DllImport ("WebInterface")] unsafe public static extern void wiSetEntityData(int num, wiEntityData *data);
    [DllImport ("WebInterface")] public static extern void wiClearEntityData();




    // utilities

    public struct wiActionData
    {
        public Vector3 mouse;
        public wiComponent target;

        public static wiActionData Build(Dictionary<String,String> dic)
        {
            wiActionData ret = new wiActionData();
            var entities = wiSystem.GetInstance().entities;
            foreach (var pair in dic)
            {
                try
                {
                    string key = pair.Key;
                    string value = pair.Value;
                    switch (key)
                    {
                        case "mouseX":
                            {
                                ret.mouse.x = Convert.ToSingle(value);
                            }
                            break;
                        case "mouseY":
                            {
                                ret.mouse.z = Convert.ToSingle(value);
                            }
                            break;
                        case "target":
                            {
                                int id = Convert.ToInt32(value);
                                entities.TryGetValue(id, out ret.target);
                            }
                            break;
                    }
                }
                catch (Exception) { }
            }
            return ret;
        }
    }
}
