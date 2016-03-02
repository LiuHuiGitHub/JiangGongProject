using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UserSerialPort
{
    enum CE_ENUM
    {
        CE_OK,
        CE_CHECK_ERROR,
        CE_BAD_COMMAND,
        CE_BAD_PARAM,
        CE_BAD_PADATA,
    };
    /* 标准数据格式：0xAA + LEN + CMD + DATA + VERIFY + 0xCC（DATA可为空）
     *      i.	LEN: CMD + DATA + VERIFY (一个字节)
     *      ii.	VERIFY: LEN XOR CMD XOR DATA（一个字节）
     * 回应格式：    0xBB ＋ CMD ＋ VALUE ＋ 0xCC ＋ 标准数据格式
     */

    class ComStdClass
    {
        private const int MAX_DATA_LEN = 128;
        public Byte head;
        public Byte len;
        public Byte cmd;
        public Byte[] dat = new Byte[MAX_DATA_LEN];
        public Byte check;
        public Byte end;

        public bool trig;
        public Byte Length { get { return (Byte)(len + 3); } }

        private void init(Byte _cmd, Byte[] _dat, Byte _len)
        {
            head = 0xAA;
            len = (Byte)(2 + _len);
            cmd = _cmd;
            dat = new Byte[MAX_DATA_LEN];
            Array.Copy(_dat, dat, _len);
            check = GetCheck();
            end = 0xCC;
            trig = false;
        }
        public ComStdClass()
        {
            Byte[] __dat = new Byte[1];
            init((Byte)0, __dat, (Byte)0);
        }
        public ComStdClass(Byte _cmd)
        {
            Byte[] __dat = new Byte[1];
            init(_cmd, __dat, (Byte)0);
        }
        public ComStdClass(Byte _cmd, Byte[] _dat, Byte _len)
        {
            init(_cmd, _dat, _len);
        }
        public ComStdClass(Byte _cmd, Byte _dat)
        {
            Byte[] __dat = new Byte[1];
            __dat[0] = _dat;
            init(_cmd, __dat, (Byte)1);
        }
        public ComStdClass(Byte _cmd, ushort _dat)
        {
            Byte[] __dat = new Byte[2];
            __dat[0] = (Byte)(_dat / 256);
            __dat[1] = (Byte)(_dat % 256);
            init(_cmd, __dat, (Byte)2);
        }
        /*获取接收数据*/
        public bool getArray(Byte[] bytes, int length)
        {
            if (length < 4)
            {
                return false;
            }
            try
            {
                head = bytes[0];
                len = bytes[1];
                cmd = bytes[2];
                for (int i = 0; i < length - 2; i++)
                {
                    if (i < MAX_DATA_LEN)
                    {
                        dat[i] = bytes[i + 3];
                    }
                    else
                    {
                        return false;
                    }
                }
                check = bytes[len + 1];
                end = bytes[len + 2];
                return true;
            }
            catch
            {
                return false;
            }
        }
        /*接收校验成功*/
        public bool Checked()
        {
            if (head == 0xAA
                && GetCheck() == check
                && end == 0xCC
                )
            {
                return true;
            }
            return false;
        }
        /*计算校验值*/
        public Byte GetCheck()
        {
            Byte i, tmpCheck = 0;
            tmpCheck ^= len;
            tmpCheck ^= cmd;
            for (i = 0; i < len - 2; i++)
            {
                tmpCheck ^= dat[i];
            }
            return tmpCheck;
        }
        /*更新校验值*/
        public void updateCheck()
        {
            check = GetCheck();
        }
        public Byte[] toArray()
        {
            Byte[] tmpBytes = new Byte[len + 3];
            tmpBytes[0] = head;
            tmpBytes[1] = len;
            tmpBytes[2] = cmd;
            for (int i = 0; i < len - 2; i++)
            {
                tmpBytes[i + 3] = dat[i];
            }
            tmpBytes[len + 1] = check;
            tmpBytes[len + 2] = end;
            return tmpBytes;
        }
        override public string ToString()
        {
            return System.Text.Encoding.Default.GetString(toArray());
        }
    };
    class ComRxClass
    {
        private const int MAX_DATA_LEN = 128;
        public Byte head;
        public Byte cmd;
        public Byte value;
        public Byte end;
        public ComStdClass std;
        public bool expand;
        public bool update;
        public ComRxClass()
        {
            head = 0xBB;
            cmd = 0;
            value = 0;
            end = 0xCC;
            std = new ComStdClass();
            expand = false;
            update = false;
        }
        /*获取接收数据*/
        public bool getArray(Byte[] bytes, int length)
        {
            try
            {
                head = bytes[0];
                cmd = bytes[1];
                value = bytes[2];
                end = bytes[3];
                if (length > 4)
                {
                    std.head = bytes[4];
                    std.len = bytes[5];
                    std.cmd = bytes[6];
                    for (int i = 0; i < std.len - 2; i++)
                    {
                        if (i < MAX_DATA_LEN)
                        {
                            std.dat[i] = bytes[i + 7];
                        }
                        else
                        {
                            return false;
                        }
                    }
                    std.check = bytes[std.len + 5];
                    std.end = bytes[std.len + 6];
                    expand = true;
                }
                else
                {
                    expand = false;
                }
                return true;
            }
            catch
            {
                return false;
            }
        }
        public bool Checked()
        {
            if (head == 0xBB
                && end == 0xCC
                )
            {
                if (expand == false)
                {
                    return true;
                }
                else if (std.Checked())
                {
                    return true;
                }
            }
            return false;
        }
        public Byte[] toArray()
        {
            Byte[] tmpArray;
            if (expand)
            {
                tmpArray = new Byte[std.len + 7];
            }
            else
            {
                tmpArray = new Byte[4];
            }
            tmpArray[0] = head;
            tmpArray[1] = cmd;
            tmpArray[2] = value;
            tmpArray[3] = end;
            if (expand)
            {
                tmpArray[4] = std.head;
                tmpArray[5] = std.len;
                tmpArray[6] = std.cmd;
                for (int i = 0; i < std.len - 2; i++)
                {
                    tmpArray[i + 7] = std.dat[i];
                }
                tmpArray[std.len + 5] = std.check;
                tmpArray[std.len + 6] = std.end;
            }
            return tmpArray;
        }
        override public string ToString()
        {
            return System.Text.Encoding.ASCII.GetString(toArray());
        }
    };
}
