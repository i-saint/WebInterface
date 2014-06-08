using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class wiSystem
{
    static wiSystem instance;

    public static wiSystem GetInstance()
    {
        if (instance == null)
        {
            instance = new wiSystem();
        }
        return instance;
    }


    Dictionary<int, GameObject> objects;
    int IDSeed = 0;

    wiSystem()
    {
        objects = new Dictionary<int, GameObject>();
    }

    public int AddObject(GameObject obj)
    {
        int id = IDSeed++;
        objects.Add(id, obj);
        return id;
    }

    public void DeleteObject(int id)
    {
        objects.Remove(id);
    }

    public void ClearObjects()
    {
        objects.Clear();
    }

    public void Update()
    {

    }
}

public class wiServer
{




    void OnDestroy()
    {
        WebInterface.wiStopServer();
    }

    void Reset()
    {
        WebInterface.wiStopServer();
    }

    void Start ()
    {
        WebInterface.wiStartServer();
    }
    
    void Update ()
    {
        foreach(KeyValuePair<int,GameObject> kvp in objects) {

        }
    }
}
