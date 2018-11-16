using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine.Tilemaps;

enum MessageID
{
    e_id_packetBegin = 134,

    ID_GAME_MESSAGE_1,
    UPDATE_NETWORK_PLAYER, // Client sends server a single entity
    UPDATE_GAME_STATE, // Server sends client update of all current entities

    // ****TO-DO: implement general identifiers
    // these should be peer-type-agnostic, i.e. either server or client
    // some suggestions: 
    //	e_id_stateInput,	// packet data contains remote input
    //	e_id_stateData,		// packet data contains info about game state
    // etc.

    // end; indicates where specialized managers can start
    e_id_packetEnd
}

public class NetworkManager : MonoBehaviour
{
    public bool enableNetworking;
    public SceneManager sceneManager; // TODO: Replace with entity manager for final.
    public string ip = "127.0.0.1";

    [DllImport("egp-net-plugin-Unity")]
    private static extern int foo(int bar);
    [DllImport("egp-net-plugin-Unity")]
    private static extern bool initNetworking(IntPtr ip);
    [DllImport("egp-net-plugin-Unity")]
    private static extern bool disconnect();
    [DllImport("egp-net-plugin-Unity")]
    private static extern bool shutdownNetworking();
    [DllImport("egp-net-plugin-Unity")]
    private static extern IntPtr handlePacket(ref int length);
    [DllImport("egp-net-plugin-Unity")]
    private static extern IntPtr plsreturn(ref int length);
    [DllImport("egp-net-plugin-Unity")]
    private static extern bool sendEntityToServer(int guidSize, byte[] guid, SimpleVector3 position, SimpleVector3 destination);

    // We want to send data to the server 10 times a second
    //private const float networkTickRateMS = 100.0f / 1000.0f;
    private const float networkTickRateMS = 500.0f / 1000.0f; //Debug: twice a second
    private float lastNetworkUpdate;

    // Start is called before the first frame update
    void Start()
    {
        if (enableNetworking)
        {
            lastNetworkUpdate = Time.time;
            IntPtr ipPtr = Marshal.StringToHGlobalAnsi(ip);
            if (initNetworking(ipPtr))
                Debug.Log("Connecting");
            else
                Debug.Log("Could not connect");
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (enableNetworking)
        {
            // If it's been the time interval we send updates at
            //Debug.Log("Current Time: " + Time.time + " Last Update + Interval: " + lastNetworkUpdate + networkTickRateMS);
            if (Time.time >= lastNetworkUpdate + networkTickRateMS)
            {
                //Debug.Log("Network Update");
                // Reset the timer
                lastNetworkUpdate = Time.time;
                // Debug Test
                SendEntity(SceneManager.localPlayer);
            }

            // Check for and queue packets as fast and often as we can.
            HandleNetworking();
        }
    }

    void SendEntity(Entity send)
    {
        byte[] guidBytes = send.identifier.ToByteArray();
        int guidSize = guidBytes.Length;

        SimpleVector3 position;
        position.x = send.transform.position.x;
        position.y = send.transform.position.y;
        position.z = send.transform.position.z;

        SimpleVector3 destination;
        destination.x = send.moveDestination.x;
        destination.y = send.moveDestination.y;
        destination.z = send.moveDestination.z;

        sendEntityToServer(guidSize, guidBytes, position, destination);
    }

    private void HandleNetworking()
    {
        int length = 0;
        IntPtr returnPtr = handlePacket(ref length);

        byte[] returnData = new byte[length];
        int index = 0;

        Marshal.Copy(returnPtr, returnData, 0, length);

        int messageID = returnData[index];
        index += 4;

        switch (messageID)
        {
           case (int)MessageID.UPDATE_NETWORK_PLAYER:
           {
                //Debug.Log("Update Network Player");
                
                int guidLength = returnData[index];
                index++;

                UInt64 latency;
                
                Guid identifer = bytesToGuid(returnData, index, guidLength);
                index += guidLength;
                
                Vector3 position = new Vector3();
                position.z = bytesToFloat(returnData, index);
                index += 4;
                position.y = bytesToFloat(returnData, index);
                index += 4;
                position.x = bytesToFloat(returnData, index);
                index += 4;
                
                Vector3 destination = new Vector3();
                destination.z = bytesToFloat(returnData, index);
                index += 4;
                destination.y = bytesToFloat(returnData, index);
                index += 4;
                destination.x = bytesToFloat(returnData, index);
                index += 4;
                
                EntityPacket newPacket;
                newPacket.identifier = identifer;
                newPacket.position = position;
                newPacket.destination = destination;
                newPacket.latency = 0.0f; // get latency from returnData

                //Debug.Log(identifer);

                if (sceneManager)
                {
                    SceneManager.entityPackets.Enqueue(newPacket);
                }

                //Debug.Log("Done");
            }
                break;
            case 110: // If there isn't nodata.
            {
            }
                break;
            default:
            {
                Debug.Log("Unknown MessageID: " + messageID);
            }
                break;
        }
        
    }

    private float bytesToFloat(byte[] data, int startIndex)
    {
        byte[] reversed = new byte[4];
        for (int i = 0; i < 4; i++)
        {
            reversed[i] = data[startIndex + 3 - i];
        }
        return BitConverter.ToSingle(reversed, 0);
    }

    private Guid bytesToGuid(byte[] data, int startIndex, int length)
    {
        //Debug.Log("Guid Length: " + length);
        byte[] guidBytes = new byte[length];
        for (int i = 0; i < length; i++)
        {
            guidBytes[i] = data[startIndex + i];
        }

        Guid newGuid = new Guid(guidBytes);

        return newGuid;
    }

    private void logEntityPacket(EntityPacket e)
    {
        Debug.Log("Guid: " + e.identifier.ToString());
        Debug.Log("Position: " + e.position);
        Debug.Log("Destination: " + e.destination);
    }

    void OnApplicationQuit()
    {
        Debug.Log("Disconnect Status: " + disconnect());
        Debug.Log("NetClosing Status: " + shutdownNetworking());
    }
}

public struct EntityPacket
{
    public Guid identifier;
    public Vector3 position;
    public Vector3 destination;
    public float latency;
}

[StructLayout(LayoutKind.Sequential)]
struct SimpleVector3
{
    public float x;
    public float y;
    public float z;
}