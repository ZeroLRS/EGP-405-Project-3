using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkPlayer : Entity
{
    public float latency;
    public bool deadReckon;
    public Vector3 serverPos;

    public bool initNetworkPlayer(Guid _identifer, Vector3 _transformPosition, Vector3 _moveDestination)
    {
        identifier = _identifer;
        transform.position = _transformPosition;
        moveDestination = _moveDestination;
        return true;
    }

    // Start is called before the first frame update
    override protected void EntityStart()
    {
    }

    // Update is called once per frame
    override protected void EntityUpdate()
    {
        if (deadReckon)
        {
            DeadReckoning();
        }
        else
        {
            move();
        }
        moveDirection = moveDestination - transform.position;
        moveDirection.Normalize();
    }

    void DeadReckoning()
    {
        // If we have a destination, move towards it
        if (Vector3.Distance(transform.position, moveDestination) > 0.05f
            && transform.position != moveDestination)
        {
            Vector3 Pt = serverPos;
            Vector3 PtDeriv = moveDirection * (speed);
            //Pt.x += (moveDirection.x) + (speed * Time.deltaTime);
            //Pt.y += (moveDirection.y) + (speed * Time.deltaTime);

            Vector3 destination = Pt + (PtDeriv * (Time.deltaTime + latency));
            serverPos = destination;
            transform.position = blend(destination);

            // If we're close enough, snap us to the exact position
            if (Vector3.Distance(transform.position, moveDestination) < 0.05f)
            {
                transform.position = moveDestination;
            }
        }
    }

    Vector3 blend(Vector3 newServerPos)
    {
        Vector3 Pt = transform.position;

        if (serverPos == Pt)
            return serverPos;

        Vector3 blendPos = (Pt + serverPos) / 2;

        return blendPos;
    }

}
