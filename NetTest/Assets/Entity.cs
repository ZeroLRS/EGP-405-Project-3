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
    
    public Vector3 velocity;
    public Vector3 moveDirection;
    public Vector3 moveDestination;

    protected bool lastDirUp;
    protected bool lastDirRight;

    // Start is called before the first frame update
    void Start()
    {
        velocity = Vector3.zero;
        moveDirection = Vector3.zero;
        moveDestination = transform.position;
        lastDirUp = true;
        lastDirRight = true;
        EntityStart();
    }

    // Update is called once per frame
    void Update()
    {
        // If we have a destination, move towards it at a constant speed
        if (Vector3.Distance(transform.position, moveDestination) > 0.01f
            && transform.position != moveDestination)
        {
            transform.position = Vector3.MoveTowards(transform.position, moveDestination, 2.0f * Time.deltaTime);
            // If we're close enough, snap us to the exact position
            if (Vector3.Distance(transform.position, moveDestination) < 0.01f)
            {
                transform.position = moveDestination;
            }
        }

        // Change sprite based on move direction
        if (moveDirection != Vector3.zero)
        {
            if (moveDirection.y > 0)
            {
                if (moveDirection.x > 0)
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
                if (moveDirection.x > 0)
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

    protected abstract void EntityUpdate();
    protected abstract void EntityStart();
}
