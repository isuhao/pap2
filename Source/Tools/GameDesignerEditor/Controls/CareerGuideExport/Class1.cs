using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Data;
using System.IO;
using System.Collections;

namespace CareerGuideExport
{
    public class Class1
    {
        public Class1()
        {

        }

        public bool Export(SqlConnection Conn, string RootDir)
        {
            bool bResult = true;
            StringBuilder sb = new StringBuilder();
            sb.Append("LinkID\tMapID\tNpcID\tKind\tx\ty\tz\tNpcName\r\n");

            String sql = "Select * from CareerGuide";
            DataTable tbl = Helper.GetDataTable(sql, Conn);
            
            //按地图划分
            sql = "Select MapID from CareerGuide where MapID > 0 group by MapID";
            DataTable tblMapID = Helper.GetDataTable(sql, Conn);
            ArrayList alMapID = new ArrayList();
            foreach (DataRow dr in tblMapID.Rows)
            {
                alMapID.Add((object)dr["MapID"].ToString());
                DataRow[] rows = tbl.Select("MapID = " + dr["MapID"].ToString());

                sql = "select Name from MapList where ID = " + dr["MapID"].ToString();
                DataTable dt = Helper.GetDataTable(sql, Conn);
                String strMapName = dt.Rows[0][0].ToString();
                if (strMapName == "测试1")
                    strMapName = RootDir + "\\data\\source\\maps\\测试\\" + strMapName + ".Map.Logical";
                else
                    strMapName = RootDir + "\\data\\source\\maps\\" + strMapName + "\\" + strMapName + ".Map.Logical";

                String strRetVal;
                String strContent = Helper.FileToString(strMapName);
                Misc.IniStructure m_inis = new Misc.IniStructure();
                m_inis = Misc.IniStructure.ReadIniWithContent(strContent);

                //遍历Logical文件中npc
                strRetVal = m_inis.GetValue("MAIN", "NumNPC");
                int nNPCCount = Int32.Parse(strRetVal);
                for (int nIndex = 0; nNPCCount > nIndex; nIndex++)
                {
                    String strSectionName = "NPC" + nIndex.ToString();
                    strRetVal = m_inis.GetValue(strSectionName, "nTempleteID");
                    int nTempleteID = Int32.Parse(strRetVal);
                    
                    foreach (DataRow row in rows)
                    {
                        if (nTempleteID == Int32.Parse(row["NpcID"].ToString()))
                        {
                        strRetVal = m_inis.GetValue(strSectionName, "nX");
                        row["x"] = strRetVal.ToString();
                        strRetVal = m_inis.GetValue(strSectionName, "nY");
                        row["y"] = strRetVal.ToString();
                        strRetVal = m_inis.GetValue(strSectionName, "nZ");
                        row["z"] = strRetVal.ToString();
                        }
                    }
                }
            }

            foreach (DataRow row in tbl.Rows)
            {
                sb.Append(row["LinkID"].ToString() + "\t");
                sb.Append(row["MapID"].ToString() + "\t");
                sb.Append(row["NpcID"].ToString() + "\t");
                sb.Append(row["Kind"].ToString() + "\t");
                sb.Append(row["x"].ToString() + "\t");
                sb.Append(row["y"].ToString() + "\t");
                sb.Append(row["z"].ToString() + "\t");
                sb.Append(row["NpcName"].ToString() + "\r\n");
                if (row["NpcID"].ToString() != "0" && 
                    (row["x"].ToString() == "" ||
                   row["y"].ToString() == "" ||
                   row["z"].ToString() == ""))
                {
                    Console.Write("NPC:" + row["NpcID"].ToString() + "未找到对应坐标！\n");
                }
            }

            sql = "Select filename from sys_export_table_cfg where tablename = 'CareerGuide'";
            tbl = Helper.GetDataTable(sql, Conn);
            String strFileName = tbl.Rows[0][0].ToString();
            strFileName = RootDir + strFileName;
            Helper.StringToFile(sb.ToString(), strFileName, Encoding.GetEncoding("gb2312"));

            return bResult;
        }
    }
};