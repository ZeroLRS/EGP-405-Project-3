using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Entity : MonoBehaviour
{
    public Guid identifier;

    // Rendering
    public Sprite neSprite, nwSprite, seSprite, swSprite;
    public SpriteRenderer spriteRenderer;

    public float speed;
    //public Vector3 velocity;
    public Vector3 moveDirection;
    public Vector3 moveDestination;

    public bool deadReckon;
    protected bool lastDirUp;
    protected bool lastDirRight;

    public Transform serverPosTest;

    // Start is called before the first frame update
    void Start()
    {
        //velocity = Vector3.zero;
        moveDirection = Vector3.zero;
        moveDestination = transform.position;
        lastDirUp = true;
        lastDirRight = true;
        EntityStart();
    }

    // Update is called once per frame
    void Update()
    {
        if(deadReckon)
        {
            DeadReckoning();

            if (serverPosTest)
            {
                MoveTest();

                if (Input.GetKeyDown(KeyCode.Space))
                {
                    transform.position = BlendWithReckoning(serverPosTest.position);
                }
            }
        }
        else
            Move();

        // Change sprite based on move direction
        if (moveDirection != Vector3.zero)
        {
            if (moveDirection.y < 0)
            {
                if (moveDirection.x < 0)
                {
                    spriteRenderer.sprite = swSprite;
                }
                else
                {
                    spriteRenderer.sprite = seSprite;
                }
            }
            else
            {
                if (moveDirection.x < 0)
                {
                    spriteRenderer.sprite = nwSprite;
                }
                else
                {
                    spriteRenderer.sprite = neSprite;
                }
            }
        }
        EntityUpdate();
    }

    void Move()
    {

        // If we have a destination, move towards it at a constant speed
        if (Vector3.Distance(transform.position, moveDestination) > 0.01f
            && transform.position != moveDestination)
        {
            transform.position = Vector3.MoveTowards(transform.position, moveDestination, speed * Time.deltaTime);
            // If we're close enough, snap us to the exact position
            if (Vector3.Distance(transform.position, moveDestination) < 0.01f)
            {
                transform.position = moveDestination;
            }
        }
    }

    void MoveTest()
    {
        // If we have a destination, move towards it at a constant speed
        if (Vector3.Distance(serverPosTest.position, moveDestination) > 0.01f
            && serverPosTest.position != moveDestination)
        {
            serverPosTest.position = Vector3.MoveTowards(serverPosTest.position, moveDestination, 0.5f * speed * Time.deltaTime);
            // If we're close enough, snap us to the exact position
            if (Vector3.Distance(serverPosTest.position, moveDestination) < 0.01f)
            {
                serverPosTest.position = moveDestination;
            }
        }
    }
    void DeadReckoning()
    {
        // If we have a destination, move towards it
        if (Vector3.Distance(transform.position, moveDestination) > 0.05f
            && transform.position != moveDestination)
        {
            Vector3 Pt = transform.position;
            Vector3 PtDeriv = moveDirection * (speed);
            //Pt.x += (moveDirection.x) + (speed * Time.deltaTime);
            //Pt.y += (moveDirection.y) + (speed * Time.deltaTime);

            Vector3 destination = Pt + (PtDeriv * Time.deltaTime);
            transform.position = Vector3.MoveTowards(transform.position, destination, speed * Time.deltaTime);

            // If we're close enough, snap us to the exact position
            if (Vector3.Distance(transform.position, moveDestination) < 0.05f)
            {
                transform.position = moveDestination;
            }
        }
    }

    public void UpdateState(EntityPacket packet)
    {
        if(identifier != packet.identifier)
            identifier = packet.identifier;

        moveDestination = packet.destination;

        if (deadReckon)
            BlendWithReckoning(packet.position);
        else
            transform.position = packet.position;
    }

    Vector3 BlendWithReckoning(Vector3 serverPos)
    {
        Vector3 Pt = transform.position;

        if (serverPos == Pt)
            return serverPos;

        Vector3 blendPos = (Pt + serverPos) / 2;

        return blendPos;
    }

    protected abstract void EntityUpdate();
    protected abstract void EntityStart();
}
