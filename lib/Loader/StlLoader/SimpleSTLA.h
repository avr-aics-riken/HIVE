/**
 * @file SimpleSTLA.h
 * STL形式ファイル簡易ローダー[アスキーデータ版]
 */
#ifndef INCLUDE_SIMPLESTLA
#define INCLUDE_SIMPLESTLA

/*
    SimpleSTLA
*/

#include "SimpleSTL.h"


/**
 * SimpleSTLA
 */
class SimpleSTLA : public SimpleSTL
{
public:
    SimpleSTLA() : SimpleSTL() {}
	~SimpleSTLA(){}
	
	bool Load(const char* filename, bool swapEndian = false, bool generateIndex = false)
	{
        (void)swapEndian; // swapEndian is not used for ASCII format.

        // read file all
		FILE* fp = fopen(filename, "rb");
		if (!fp)
			return false;
		fseek(fp, 0, SEEK_END);
		size_t byte_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (byte_size == 0){
			fclose(fp);
			return false;
		}
		char* data = new char[byte_size];
		fread(data,byte_size,1,fp);
		fclose(fp);
	
        
        const char* solidstr    = "solid";
        const char* endsolidstr = "endsolid";
        const char* facetstr    = "facet normal";
        const char* endfacestr  = "endfacet";
        const char* outerstr    = "outer loop";
        const char* endouterstr = "endloop";
        const char* vertexstr   = "vertex";
        
        size_t i = 0;
        while (true)
        {
            i += skipSpace(&data[i]);
            
            if(i >= byte_size){
                break;
            }
            
            if (strncmp(&data[i], solidstr, strlen(solidstr)) == 0)
            {
                i += searchReturn(&data[i]);
            }
            else if (strncmp(&data[i], facetstr, strlen(facetstr)) == 0)
            {
                i += strlen(facetstr);
                vec3 normal;
                i += step_fatof(&data[i], &normal.x);
                i += step_fatof(&data[i], &normal.y);
                i += step_fatof(&data[i], &normal.z);
                m_normal.push_back(normal);
                m_normal.push_back(normal);
                m_normal.push_back(normal);
                i += searchReturn(&data[i]);
            }
            else if (strncmp(&data[i], vertexstr, strlen(vertexstr)) == 0)
            {
                i += strlen(vertexstr);
                vec3 pos;
                i += step_fatof(&data[i], &pos.x);
                i += step_fatof(&data[i], &pos.y);
                i += step_fatof(&data[i], &pos.z);
                m_pos.push_back(pos);
                
                i += searchReturn(&data[i]);
            }
            else if (strncmp(&data[i], outerstr, strlen(outerstr)) == 0)
            {
                i += searchReturn(&data[i]);
            }
            else if (strncmp(&data[i], endouterstr, strlen(endouterstr)) == 0)
            {
                i += searchReturn(&data[i]);
            }		
            else if (strncmp(&data[i], endfacestr, strlen(endfacestr)) == 0)
            {
                i += searchReturn(&data[i]);
            }
            else if (strncmp(&data[i], endsolidstr, strlen(endsolidstr)) == 0)
            {
                //break;
                
                // there are many solids
                // cotinue
                i += searchReturn(&data[i]);
            }
            else // unknown line
            {
                i += searchReturn(&data[i]);
            }
        }
        
        const unsigned int vertexCount = m_pos.size();
        const unsigned int triCount = vertexCount / 3;
		
		if (generateIndex) {
			for (unsigned int i = 0; i < triCount; i++)
			{
				m_face[i].i0 = 3*i  ;
				m_face[i].i1 = 3*i+1;
				m_face[i].i2 = 3*i+2;
			}
		}
		
		delete [] data;
		return true;
	}
	
};

#endif // INCLUDE_SIMPLESTLA

