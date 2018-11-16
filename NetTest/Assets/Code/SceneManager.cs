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

    public static Queue<EntityPacket> entityPackets;

    // Start is called before the first frame update
    void Start()
    {
        floorLayer = GameObject.Find("Floor").GetComponent<Tilemap>();
        entityLayer = GameObject.Find("Entities").GetComponent<Tilemap>();
        wallLayer = GameObject.Find("Wall").GetComponent<Tilemap>();

        localPlayer = GameObject.Find("TestPlayer").GetComponent<LocalPlayer>();

        entities = new List<Entity>();
        entityPackets = new Queue<EntityPacket>();

        //GameObject testEnemy = GameObject.Instantiate<GameObject>(Resources.Load<GameObject>("Entities/TestEnemy"));
        //testEnemy.transform.SetParent(entityLayer.transform);
        //testEnemy.transform.localPosition = new Vector3(3, 3, 0);
        //entities.Add(testEnemy.GetComponent<Entity>());
    }

    // Update is called once per frame
    void Update()
    {
        UpdateEntityStates();

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

    public void UpdateEntityStates()
    {
        //Create queue for entities that are not found and fill with data from server
        //Queue<EntityPacket> entitiesNotFound = entityData;//new Queue<EntityPacket>(entityData);

        //Update player data
        findAndUpdateData(localPlayer, entityPackets);

        //Update other entities
        foreach(Entity entity in entities)
        {
            findAndUpdateData(entity, entityPackets);
        }

        //Create entities for remaining packets
        while(entityPackets.Count > 0)
        {
            CreateEntityFromPacket(entityPackets.Dequeue());
        }
    }

    bool findAndUpdateData(Entity entity, Queue<EntityPacket> serverData)
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
        
        //Debug.LogWarning("Packet was not found for entity " + entity.gameObject.name + ". Unable to update");

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

        entities.Add(newEntity);

        newEntity.transform.position = data.position;
        newEntity.moveDestination = data.destination;
        //newEntity.deadReckon = true;
        newEntity.acceleration = 2.0f;
        newEntity.UpdateState(data);
    }
}
