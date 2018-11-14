using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine.Tilemaps;

public class NetworkManager : MonoBehaviour
{
    public bool enableNetworking;

    [DllImport("egp-net-plugin-Unity")]
    private static extern int foo(int bar);
    [DllImport("egp-net-plugin-Unity")]
    private static extern bool initNetworking();
    [DllImport("egp-net-plugin-Unity")]
    private static extern IntPtr handlePacket(ref int length);
    [DllImport("egp-net-plugin-Unity")]
    private static extern IntPtr plsreturn(ref int length);

    // Start is called before the first frame update
    void Start()
    {
        if (enableNetworking)
        {
            Debug.Log(initNetworking());
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (enableNetworking)
        {
            netTest();
        }
    }

    private void netTest()
    {
        int length = 0;
        int i = 0;
        IntPtr returnPtr = handlePacket(ref length);
        byte[] returnData = new byte[length];

        Marshal.Copy(returnPtr, returnData, 0, length);

        int id = returnData[i];
        i++;

        if (returnData[0] == 136)
        {
            Debug.Log("Update Network Player");

            //for (; i < 5; i++)
            //{
            //    Debug.Log(returnData[i]);
            //}

            Vector3 zoop = new Vector3();
            zoop.x = bytesToFloat(returnData, i);
            i += 4;
            zoop.y = bytesToFloat(returnData, i);
            i += 4;
            zoop.z = bytesToFloat(returnData, i);
            i += 4;

            Debug.Log(zoop);
            Debug.Log("Done");

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
}
