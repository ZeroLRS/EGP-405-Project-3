using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LocalPlayer : Entity
{
    
    // Start is called before the first frame update
    override protected void EntityStart()
    {
        identifier = Guid.NewGuid();
    }

    // Update is called once per frame
    override protected void EntityUpdate()
    {
        if (Input.GetMouseButton(0))
        {
            Vector3 attemptMove = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            attemptMove = new Vector3(attemptMove.x, attemptMove.y, 0);

            // Make sure we don't try to move into a wall or off of the floor
            if (!SceneManager.checkWallCollision(attemptMove)
                && SceneManager.checkFloorCollision(attemptMove))
            {
                moveDestination = attemptMove;
                moveDirection = transform.position - moveDestination;
                moveDirection.Normalize();
            }

        }
    }

}
