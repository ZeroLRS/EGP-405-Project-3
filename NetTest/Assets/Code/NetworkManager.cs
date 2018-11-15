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

    [DllImport("egp-net-plugin-Unity")]
    private static extern int foo(int bar);
    [DllImport("egp-net-plugin-Unity")]
    private static extern bool initNetworking(/*byte[] ip*/);
    [DllImport("egp-net-plugin-Unity")]
    private static extern IntPtr handlePacket(ref int length);
    [DllImport("egp-net-plugin-Unity")]
    private static extern IntPtr plsreturn(ref int length);
    [DllImport("egp-net-plugin-Unity")]
    private static extern bool sendEntityToServer(int guidSize, byte[] guid, SimpleVector3 position, SimpleVector3 destination);

    // Start is called before the first frame update
    void Start()
    {
        if (enableNetworking)
        {
            char[] ip = ("127.0.0.1").ToCharArray();
            byte[] ipBytes = new byte[ip.Length];
            for (int i = 0; i < ip.Length; i++)
            {
                ipBytes[i] = (byte)ip[i];
            }
            if (initNetworking(/*ipBytes*/))
                Debug.Log("asdsda");
            else
                Debug.Log("why");
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (enableNetworking)
        {
            HandleNetworking();
            //SendEntity(SceneManager.localPlayer);
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
        index++;

        if (messageID == (int)MessageID.UPDATE_NETWORK_PLAYER)
        {
            Debug.Log("Update Network Player");

            //int guidLength = returnData[index];
            //index++;
            //
            //Guid identifer = bytesToGuid(returnData, index, guidLength);
            //index += guidLength;
            //
            //Vector3 position = new Vector3();
            //position.x = bytesToFloat(returnData, index);
            //index += 4;
            //position.y = bytesToFloat(returnData, index);
            //index += 4;
            //position.z = bytesToFloat(returnData, index);
            //index += 4;
            //
            //Vector3 destination = new Vector3();
            //destination.x = bytesToFloat(returnData, index);
            //index += 4;
            //destination.y = bytesToFloat(returnData, index);
            //index += 4;
            //destination.z = bytesToFloat(returnData, index);
            //index += 4;
            //
            //EntityPacket newPacket;
            //newPacket.identifier = identifer;
            //newPacket.position = position;
            //newPacket.destination = destination;
            
            Debug.Log("Done");

        }
        else if (messageID != 110) // If there isn't nodata.
        {
            Debug.Log("Unknown MessageID: " + messageID);
        }

        //Debug.Log(Encoding.ASCII.GetString(returnData));
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
        byte[] guidBytes = new byte[length];
        Guid newGuid = new Guid(guidBytes);

        return newGuid;
    }

    private void logEntityPacket(EntityPacket e)
    {
        Debug.Log("Guid: " + e.identifier.ToString());
        Debug.Log("Position: " + e.position);
        Debug.Log("Destination: " + e.destination);
    }
}

public struct EntityPacket
{
    public Guid identifier;
    public Vector3 position;
    public Vector3 destination;
}

[StructLayout(LayoutKind.Sequential)]
struct SimpleVector3
{
    public float x;
    public float y;
    public float z;
}