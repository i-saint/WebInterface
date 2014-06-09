using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class wiComponent : MonoBehaviour {

    public delegate void Handler(wi.wiEventType type, Dictionary<String, String> kvp);
    public Handler eventHandler;
    public Vector4 color = Vector4.one*0.5f;
    int id;
    int typeID;

    public int GetWebInterfaceID() { return id; }
    public int GetWebInterfaceTypeID() { return typeID; }


    void Start ()
    {
        id = wiSystem.GetInstance().AddObject(this);
        typeID = wiSystem.GetInstance().GetTypeID(this);
    }

    void OnDestroy()
    {
        wiSystem.GetInstance().DeleteObject(id);
    }

    public void OnWebEvent(wi.wiEventType type, Dictionary<String, String> kvp)
    {
        if (eventHandler != null) { eventHandler(type, kvp); }
    }
}
