using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;


public class wiSystem
{
    public static wiSystem instance;
    public Dictionary<int, wiComponent> entities;
    int IDSeed = 0;

    public delegate void Callback(Dictionary<String, String> kvp);
    public Callback onAction;

    public static wiSystem GetInstance()
    {
        if (instance == null)
        {
            instance = new wiSystem();
        }
        return instance;
    }

    wiSystem()
    {
        entities = new Dictionary<int, wiComponent>();
    }

    public int AddObject(wiComponent obj)
    {
        int id = IDSeed++;
        entities.Add(id, obj);
        return id;
    }

    public void DeleteObject(int id)
    {
        entities.Remove(id);
    }


    public unsafe void Update()
    {
        wi.wiEntityData[] data = new wi.wiEntityData[entities.Count];
        int i = 0;
        foreach (KeyValuePair<int, wiComponent> kvp in entities)
        {
            data[i].id = kvp.Key;
            data[i].transform = kvp.Value.transform.localToWorldMatrix;
            data[i].size = Vector4.one*0.5f;
            data[i].color = kvp.Value.color;
            ++i;
        }
        fixed (wi.wiEntityData* ptr = data)
        {
            wi.wiSetEntityData(entities.Count, ptr);
            wi.wiUpdate();
        }
    }

    public unsafe void OnAction(int num, wi.wiKeyValue* kvps)
    {
        var entities = wiSystem.GetInstance().entities;
        Dictionary<String, String> dic = new Dictionary<String, String>();
        wiComponent receiver = null;
        for (int i = 0; i < num; ++i)
        {
            string name = Marshal.PtrToStringAnsi((IntPtr)kvps[i].name);
            string value = Marshal.PtrToStringAnsi((IntPtr)kvps[i].value);
            dic.Add(name, value);
            switch (name)
            {
                case "entity":
                    {
                        int id = Convert.ToInt32(value);
                        entities.TryGetValue(id, out receiver);
                    }
                    break;
            }
        }
        if (receiver) { receiver.OnAction(dic); }
        if (onAction!=null) { onAction(dic); }
    }
}

public class wiServer : MonoBehaviour
{
    void OnDestroy()
    {
        wi.wiStopServer();
    }

    void Reset()
    {
        wi.wiStopServer();
    }

    unsafe void Start ()
    {
        Application.runInBackground = true;
        wi.wiStartServer();
        wi.wiSetActionCallback((num, kvps) => wiSystem.GetInstance().OnAction(num, kvps));
    }
    
    unsafe void Update ()
    {
        wiSystem.GetInstance().Update();
    }

}
