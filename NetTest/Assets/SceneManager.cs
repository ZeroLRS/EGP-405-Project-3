using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class SceneManager : MonoBehaviour
{
    public static Tilemap floorLayer;
    public static Tilemap entityLayer;
    public static Tilemap wallLayer;
    
    public static LocalPlayer localPlayer;

    private static List<Entity> entities;

    // Start is called before the first frame update
    void Start()
    {
        floorLayer = GameObject.Find("Floor").GetComponent<Tilemap>();
        entityLayer = GameObject.Find("Entities").GetComponent<Tilemap>();
        wallLayer = GameObject.Find("Wall").GetComponent<Tilemap>();

        localPlayer = GameObject.Find("TestPlayer").GetComponent<LocalPlayer>();

        entities = new List<Entity>();
        GameObject testEnemy = GameObject.Instantiate<GameObject>(Resources.Load<GameObject>("Entities/TestEnemy"));
        testEnemy.transform.SetParent(entityLayer.transform);
        testEnemy.transform.localPosition = new Vector3(3, 3, 0);
        entities.Add(testEnemy.GetComponent<Entity>());
    }

    // Update is called once per frame
    void Update()
    {
        foreach (Entity e in entities)
        {
            if (Vector3.Distance(localPlayer.transform.position, e.transform.position) < 0.5f)
            {
                Debug.Log("FIGHT");
            }
        }
    }

    public static bool checkWallCollision(Vector3 pos)
    {
        if (wallLayer.GetTile(wallLayer.WorldToCell(pos)))
        {
            return true;
        }

        return false;
    }

    public static bool checkFloorCollision(Vector3 pos)
    {
        if (floorLayer.GetTile(floorLayer.WorldToCell(pos)))
        {
            return true;
        }

        return false;
    }
}
