using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class wiComponent : MonoBehaviour {

    public delegate void Callback(Dictionary<String, String> kvp);
    public Callback onAction;
    public Vector4 color = Vector4.one*0.5f;
    int id;

    void Start ()
    {
        id = wiSystem.GetInstance().AddObject(this);
    }

    void OnDestroy()
    {
        wiSystem.GetInstance().DeleteObject(id);
    }

    public virtual void OnAction(Dictionary<String, String> kvp)
    {
        if (onAction != null) { onAction(kvp); }
    }
}
