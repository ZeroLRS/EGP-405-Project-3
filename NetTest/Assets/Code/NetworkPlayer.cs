using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkPlayer : Entity
{
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
        moveDirection = moveDestination - transform.position;
        moveDirection.Normalize();
    }
}
