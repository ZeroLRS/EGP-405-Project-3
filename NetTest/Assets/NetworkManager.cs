using System;
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
        IntPtr returnPtr = handlePacket(ref length);
        byte[] returnData = new byte[length];

        Marshal.Copy(returnPtr, returnData, 0, length);
        Debug.Log(Encoding.ASCII.GetString(returnData));
    }
}
