using UnityEngine;
using System.Collections;

public class wiComponent : MonoBehaviour {

    public delegate void Callback(wi.wiActionData data);
    public Callback onAction;
    int id;

	void Start ()
    {
        id = wiSystem.GetInstance().AddObject(this);
    }

    void OnDestroy()
    {
        wiSystem.GetInstance().DeleteObject(id);
    }

    public virtual void OnAction(wi.wiActionData data)
    {
        if (onAction != null) { onAction(data); }
    }
}
