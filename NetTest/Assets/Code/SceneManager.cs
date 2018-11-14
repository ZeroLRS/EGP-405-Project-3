using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class SceneManager : MonoBehaviour
{
    public Entity entityPrefab;
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
        pos.y -= .25f;
        if (floorLayer.GetTile(floorLayer.WorldToCell(pos)))
        {
            return true;
        }

        return false;
    }

    public void UpdateEntityStates(List<EntityPacket> entityData)
    {
        //Create queue for entities that are not found
        Queue<EntityPacket> entitiesNotFound = new Queue<EntityPacket>();

        //Fill with data from server
        foreach(EntityPacket packet in entityData)
        {
            entitiesNotFound.Enqueue(packet);
        }

        //Update player data
        findAndUpdateData(localPlayer, ref entitiesNotFound);

        //Update other entities
        foreach(Entity entity in entities)
        {
            findAndUpdateData(entity, ref entitiesNotFound);
        }

        //Create entities for remaining packets
        while(entitiesNotFound.Count > 0)
        {
            CreateEntityFromPacket(entitiesNotFound.Dequeue());
        }
    }

    bool findAndUpdateData(Entity entity, ref Queue<EntityPacket> serverData)
    {        
        int numLoops = serverData.Count;

        while (numLoops > 0)
        {
            //Remove top packet from queue
            EntityPacket top = serverData.Dequeue();

            if (entity.identifier == top.identifier) // if entity was found
            {
                entity.UpdateState(top);

                return true;
            }
            else //if not found return top entity to queue
            {
                serverData.Enqueue(top);
            }

            --numLoops;
        }

        Debug.LogError("Packet was not found for entity " + entity.gameObject.name + ". Unable to update");

        return false;
    }

    void CreateEntityFromPacket(EntityPacket data)
    {
        Entity newEntity = Instantiate(entityPrefab).GetComponent<Entity>();
        
        if (!newEntity)
        {
            Debug.LogError("Something went wrong when creating new entity");
            return;
        }

        newEntity.transform.position = data.position;
        newEntity.UpdateState(data);
    }
}
