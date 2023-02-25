//#include <cstring>
//#include <cstdio>
//#include <string>
#include "envi.h"
#include "support/datalib.h"

int32_t read_envi_hdr(string file, envi_hdr &hdr)
{
    char inbuf[1000];
    FILE *fp;

    if (!data_isfile(file))
    {
        return COSMOS_GENERAL_ERROR_NAME;
    }

    size_t ext;
    if ((ext = file.find(".hdr")) != string::npos)
    {
        hdr.basename = file.substr(0, ext);
        hdr.dataname = hdr.basename;
        if (!data_isfile(hdr.dataname))
        {
            return COSMOS_GENERAL_ERROR_NAME;
        }
        hdr.hdrname = file;
    }
    else if (data_isfile(file + ".hdr"))
    {
        hdr.basename = file;
        hdr.hdrname = hdr.basename + ".hdr";
        if (!data_isfile(hdr.hdrname))
        {
            return COSMOS_GENERAL_ERROR_NAME;
        }
        hdr.dataname = file;
    }
    else if ((ext=file.find_last_of(".")) != string::npos)
    {
        hdr.basename = file.substr(0, ext);
        hdr.hdrname = hdr.basename + ".hdr";
        if (!data_isfile(hdr.hdrname))
        {
            return COSMOS_GENERAL_ERROR_NAME;
        }
        hdr.dataname = file;
    }
    else
    {
        return COSMOS_GENERAL_ERROR_NAME;
    }

    fp = fopen(hdr.hdrname.c_str(),"r");
    if (fp == nullptr)
    {
        return (-errno);
    }

    hdr.keys.resize(0);
    hdr.values.resize(0);
    while(fgets(inbuf,999,fp) != NULL)
    {
        size_t loc;
        string line = inbuf;

        if (line[0] != ';' && (loc=line.find(" = ")) != (string::npos))
        {
            if (inbuf[loc+3] == '{')
            {
                if ((loc=line.find("}")) == string::npos)
                {
                    while(fgets(inbuf,999,fp) != NULL)
                    {
                        line = inbuf;
                        if ((loc=line.find("}")) != string::npos)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                size_t vloc = loc + 2;
                for (size_t i=loc+2; i<line.size(); ++i)
                {
                    if (line[i] != ' ')
                    {
                        vloc = i;
                        break;
                    }
                }
                hdr.values.push_back(line.substr(vloc,line.size()-(vloc)));
                for (size_t i=loc; i>0; --i)
                {
                    if (line[i-1] != ' ')
                    {
                        loc = i;
                        break;
                    }
                }
                for (size_t i=0; i<loc; ++i)
                {
                    if (line[i] >= 'A' && line[i] <= 'Z')
                    {
                        line[i] += 'a' - 'A';
                    }
                }
                hdr.keys.push_back(line.substr(0,loc));
            }
        }
    }

    for (size_t i=0; i<hdr.keys.size(); ++i)
    {
        if (hdr.keys[i] == "samples")
        {
            sscanf(hdr.values[i].data(), "%lu",&hdr.columns);
        }
        else if (hdr.keys[i] == "lines")
        {
            sscanf(hdr.values[i].data(), "%lu",&hdr.rows);
        }
        else if (hdr.keys[i] == "bands")
        {
            sscanf(hdr.values[i].data(), "%lu",&hdr.planes);
        }
        else if (hdr.keys[i] == "header offset")
        {
            sscanf(hdr.values[i].data(), "%lu",&hdr.offset);
        }
        else if (hdr.keys[i] == "data type")
        {
            sscanf(hdr.values[i].data(), "%lu",&hdr.datatype);
        }
        else if (hdr.keys[i] == "byte order")
        {
            sscanf(hdr.values[i].data(), "%lu",&hdr.byteorder);
        }
        else if (hdr.keys[i] == "interleave")
        {
            if (!hdr.values[i].compare(0,3,"bsq") || !hdr.values[i].compare(0,3,"BSQ"))
            {
                hdr.interleave = BSQ;
            }
            else if (!hdr.values[i].compare(0,3,"bip") || !hdr.values[i].compare(0,3,"BIP"))
            {
                hdr.interleave = BIP;
            }
            else if (!hdr.values[i].compare(0,3,"bil") || !hdr.values[i].compare(0,3,"BIL"))
            {
                hdr.interleave = BIL;
            }
        }
    }

    fclose(fp);
    return (0);
}

int32_t write_envi_hdr(envi_hdr &hdr)
{
    string fname;
    FILE *fp;

    size_t extension;
    if ((extension = hdr.basename.find_last_of(".")) != string::npos)
    {
        fname = hdr.basename.substr(0, extension) + ".hdr";
    }
    else {
        fname = hdr.basename + ".hdr";
    }
    fp = fopen(fname.c_str(),"w");
    fprintf(fp,"ENVI\n");
    //    for (size_t k=0; k<hdr.keys.size(); ++k)
    //    {
    //        if (hdr.keys[k] == "description")
    //        {
    //            hdr.description = hdr.values[k];
    //        }
    //        if (hdr.keys[k] == "samples")
    //        {
    //            sscanf(hdr.values[k].data(), "%hu", &hdr.columns);
    //        }
    //        if (hdr.keys[k] == "lines")
    //        {
    //            sscanf(hdr.values[k].data(), "%hu", &hdr.rows);
    //        }
    //        if (hdr.keys[k] == "planes")
    //        {
    //            sscanf(hdr.values[k].data(), "%hu", &hdr.planes);
    //        }
    //        if (hdr.keys[k] == "header offset")
    //        {
    //            sscanf(hdr.values[k].data(), "%u", &hdr.offset);
    //        }
    //        if (hdr.keys[k] == "data type")
    //        {
    //            sscanf(hdr.values[k].data(), "%hu", &hdr.datatype);
    //        }
    //        if (hdr.keys[k] == "interleave")
    //        {
    //            if (hdr.values[k] == "BSQ" || hdr.values[k] == "bsq")
    //            {
    //                hdr.interleave = BSQ;
    //            }
    //            else if (hdr.values[k] == "BIP" || hdr.values[k] == "bip")
    //            {
    //                hdr.interleave = BIP;
    //            }
    //            else if (hdr.values[k] == "BIL" || hdr.values[k] == "bil")
    //            {
    //                hdr.interleave = BIL;
    //            }
    //        }
    //        if (hdr.keys[k] == "byte order")
    //        {
    //            sscanf(hdr.values[k].data(), "%hu", &hdr.byteorder);
    //        }
    //    }

    fprintf(fp,"description = { %s }\n", hdr.description.data());
    fprintf(fp,"samples = %lu\n",hdr.columns);
    fprintf(fp,"lines = %lu\n",hdr.rows);
    fprintf(fp,"bands = %lu\n",hdr.planes);
    fprintf(fp,"header offset = %lu\n",hdr.offset);
    fprintf(fp,"file type = ENVI Standard\n");
    fprintf(fp,"data type = %lu\n",hdr.datatype);
    switch(hdr.interleave)
    {
    case BSQ:
        fprintf(fp,"interleave = bsq\n");
        break;
    case BIP:
        fprintf(fp,"interleave = bip\n");
        break;
    case BIL:
        fprintf(fp,"interleave = bil\n");
        break;
    }
    fprintf(fp,"byte order = %lu\n",hdr.byteorder);
    if (hdr.map_info.size())
    {
        fprintf(fp, "%s\n", hdr.map_info.c_str());
    }
    else
    {
        fprintf(fp,"map info = {Arbitrary, 1.5000, 1.5000, %f, %f, %f, %f, units=Meters)\n",hdr.x0,hdr.y0,hdr.xmpp,hdr.ympp);
    }
    if (hdr.projection_info.size())
    {
        fprintf(fp, "%s\n", hdr.projection_info.c_str());
    }
    if (hdr.band_name.size())
    {
        fprintf(fp,"band names = {\n");
        for (size_t i=0; i<hdr.planes; i++)
        {
            fprintf(fp,"%s",hdr.band_name[i].data());
            if (i!=hdr.planes-1)
            {
                fprintf(fp,",\n");
            }
        }
        fprintf(fp,"}\n");
    }
    if (hdr.wavelength.size())
    {
        fprintf(fp,"wavelength = {\n");
        for (size_t i=0; i<hdr.planes; i++)
        {
            fprintf(fp,"%f",hdr.wavelength[i]);
            if (i!=hdr.planes-1)
            {
                fprintf(fp,",\n");
            }
        }
        fprintf(fp,"}\n");
    }

    fclose(fp);
    return (0);
}

int32_t read_envi_data(string fname, vector<vector<vector<double>>> &data)
{
    envi_hdr ehdr;
    int32_t iretn = 0;

    iretn = read_envi_hdr(fname, ehdr);
    if (iretn < 0)
    {
        return iretn;
    }

    switch (ehdr.interleave)
    {
    case BSQ:
        data.resize(ehdr.planes);
        for (uint16_t ip=0; ip<ehdr.planes; ++ip)
        {
            data[ip].resize(ehdr.rows);
            for (uint16_t ir=0; ir<ehdr.rows; ++ir)
            {
                data[ip][ir].resize(ehdr.columns);
            }
        }
        break;
    case BIL:
        data.resize(ehdr.rows);
        for (uint16_t ip=0; ip<ehdr.rows; ++ip)
        {
            data[ip].resize(ehdr.planes);
            for (uint16_t ir=0; ir<ehdr.planes; ++ir)
            {
                data[ip][ir].resize(ehdr.columns);
            }
        }
        break;
    case BIP:
        data.resize(ehdr.rows);
        for (uint16_t ip=0; ip<ehdr.rows; ++ip)
        {
            data[ip].resize(ehdr.columns);
            for (uint16_t ir=0; ir<ehdr.columns; ++ir)
            {
                data[ip][ir].resize(ehdr.planes);
            }
        }
        break;
    default:
        return COSMOS_GENERAL_ERROR_OUTOFRANGE;
        break;
    }

    FILE *fp = fopen(fname.c_str(), "rb");
    if (fp == nullptr)
    {
        return -errno;
    }

    uint8_t datasize=1;

    switch (ehdr.interleave)
    {
    case BSQ:
        for (size_t ip=0; ip<ehdr.planes; ++ip)
        {
            for (size_t ir=0; ir<ehdr.rows; ++ir)
            {
                switch (ehdr.datatype)
                {
                case DT_BYTE:
                    {
                        datasize = 1;
                        vector<uint8_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_INT:
                    {
                        datasize = 2;
                        vector<int16_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_U_INT:
                    {
                        datasize = 2;
                        vector<uint16_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_LONG:
                    {
                        datasize = 4;
                        vector<int32_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_U_LONG:
                    {
                        datasize = 4;
                        vector<uint32_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_FLOAT:
                    {
                        datasize = 4;
                        vector<float> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_DOUBLE:
                    datasize = 8;
                    fread(data[ip][ir].data(), datasize, ehdr.columns, fp);
                    break;
                }
            }
        }
        break;
    case BIL:
        for (size_t ir=0; ir<ehdr.rows; ++ir)
        {
            for (size_t ip=0; ip<ehdr.planes; ++ip)
            {
                switch (ehdr.datatype)
                {
                case DT_BYTE:
                    {
                        datasize = 1;
                        vector<uint8_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_INT:
                    {
                        datasize = 2;
                        vector<int16_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_U_INT:
                    {
                        datasize = 2;
                        vector<uint16_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_LONG:
                    {
                        datasize = 4;
                        vector<int32_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_U_LONG:
                    {
                        datasize = 4;
                        vector<uint32_t> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_FLOAT:
                    {
                        datasize = 4;
                        vector<float> datain(ehdr.columns);
                        fread(datain.data(), datasize, ehdr.columns, fp);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            data[ip][ir][ic] = datain[ic];
                        }
                    }
                    break;
                case DT_DOUBLE:
                    datasize = 8;
                    fread(data[ip][ir].data(), datasize, ehdr.columns, fp);
                    break;
                }
            }
        }
        break;
    case BIP:
        for (size_t ir=0; ir<ehdr.rows; ++ir)
        {
            for (size_t ic=0; ic<ehdr.columns; ++ic)
            {
                switch (ehdr.datatype)
                {
                case DT_BYTE:
                    {
                        datasize = 1;
                        vector<uint8_t> datain(ehdr.planes);
                        fread(datain.data(), datasize, ehdr.planes, fp);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            data[ip][ir][ic] = datain[ip];
                        }
                    }
                    break;
                case DT_INT:
                    {
                        datasize = 2;
                        vector<int16_t> datain(ehdr.planes);
                        fread(datain.data(), datasize, ehdr.planes, fp);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            data[ip][ir][ic] = datain[ip];
                        }
                    }
                    break;
                case DT_U_INT:
                    {
                        datasize = 2;
                        vector<uint16_t> datain(ehdr.planes);
                        fread(datain.data(), datasize, ehdr.planes, fp);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            data[ip][ir][ic] = datain[ip];
                        }
                    }
                    break;
                case DT_LONG:
                    {
                        datasize = 4;
                        vector<int32_t> datain(ehdr.planes);
                        fread(datain.data(), datasize, ehdr.planes, fp);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            data[ip][ir][ic] = datain[ip];
                        }
                    }
                    break;
                case DT_U_LONG:
                    {
                        datasize = 4;
                        vector<uint32_t> datain(ehdr.planes);
                        fread(datain.data(), datasize, ehdr.planes, fp);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            data[ip][ir][ic] = datain[ip];
                        }
                    }
                    break;
                case DT_FLOAT:
                    {
                        datasize = 4;
                        vector<float> datain(ehdr.planes);
                        fread(datain.data(), datasize, ehdr.planes, fp);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            data[ip][ir][ic] = datain[ip];
                        }
                    }
                    break;
                case DT_DOUBLE:
                    {
                        datasize = 8;
                        vector<double> datain(ehdr.planes);
                        fread(datain.data(), datasize, ehdr.planes, fp);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            data[ip][ir][ic] = datain[ip];
                        }
                    }
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    fclose(fp);
    return datasize * ehdr.planes * ehdr.columns * ehdr.rows;

}

int32_t write_envi_data(string name, uint8_t interleave, vector<vector<double>> &data)
{
    int32_t iretn = 0;
    vector<vector<vector<double>>> stack;
    stack.push_back(data);
    iretn = write_envi_data(name, interleave, stack);
    return iretn;
}

int32_t write_envi_data(string name, uint8_t interleave, vector<vector<vector<double>>> &data)
{
    envi_hdr ehdr;
    int32_t iretn = 0;

    ehdr.basename = name;
    ehdr.datatype = DT_DOUBLE;
    ehdr.interleave = interleave;
    ehdr.offset = 0;
    if (BYTE_ORDER == LITTLE_ENDIAN)
    {
        ehdr.byteorder = BO_INTEL;
    }
    else
    {
        ehdr.byteorder = BO_NETWORK;
    }
    switch (ehdr.interleave)
    {
    case BSQ:
        ehdr.planes = data.size();
        ehdr.rows = data[0].size();
        ehdr.columns = data[0][0].size();
        break;
    case BIL:
        ehdr.rows = data.size();
        ehdr.planes = data[0].size();
        ehdr.columns = data[0][0].size();
        break;
    case BIP:
        ehdr.rows = data.size();
        ehdr.columns = data[0].size();
        ehdr.planes = data[0][0].size();
        break;
    default:
        return COSMOS_GENERAL_ERROR_OUTOFRANGE;
        break;
    }

    iretn = write_envi_hdr(ehdr);
    if (iretn < 0)
    {
        return iretn;
    }

    size_t extension;
    string fname;
    if ((extension = ehdr.basename.find_last_of(".")) != string::npos)
    {
        fname = ehdr.basename;
    }
    else {
        fname = ehdr.basename.substr(0, extension);
        switch (ehdr.interleave)
        {
        case BSQ:
            fname += ".bsq";
            break;
        case BIL:
            fname += ".bil";
            break;
        case BIP:
            fname += ".bip";
            break;
        default:
            fname += ".img";
            break;
        }
    }
    FILE *fp = fopen(fname.c_str(), "wb");
    if (fp == nullptr)
    {
        return -errno;
    }

    uint8_t datasize=1;
    switch (ehdr.interleave)
    {
    case BSQ:
        for (size_t ip=0; ip<ehdr.planes; ++ip)
        {
            for (size_t ir=0; ir<ehdr.rows; ++ir)
            {
                switch (ehdr.datatype)
                {
                case DT_BYTE:
                    {
                        datasize = 1;
                        vector<uint8_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_INT:
                    {
                        datasize = 2;
                        vector<int16_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_U_INT:
                    {
                        datasize = 2;
                        vector<uint16_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_LONG:
                    {
                        datasize = 4;
                        vector<int32_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_U_LONG:
                    {
                        datasize = 4;
                        vector<uint32_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_FLOAT:
                    {
                        datasize = 4;
                        vector<float> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_DOUBLE:
                    {
                        datasize = 8;
                        fwrite(data[ip][ir].data(), datasize, ehdr.columns, fp);
                    }
                    break;
                }
            }
        }
        break;
    case BIL:
        for (size_t ir=0; ir<ehdr.rows; ++ir)
        {
            for (size_t ip=0; ip<ehdr.planes; ++ip)
            {
                switch (ehdr.datatype)
                {
                case DT_BYTE:
                    {
                        datasize = 1;
                        vector<uint8_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_INT:
                    {
                        datasize = 2;
                        vector<int16_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_U_INT:
                    {
                        datasize = 2;
                        vector<uint16_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_LONG:
                    {
                        datasize = 4;
                        vector<int32_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_U_LONG:
                    {
                        datasize = 4;
                        vector<uint32_t> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_FLOAT:
                    {
                        datasize = 4;
                        vector<float> dataout(ehdr.columns);
                        for (size_t ic=0; ic<ehdr.columns; ++ic)
                        {
                            dataout[ic] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.columns, fp);
                    }
                    break;
                case DT_DOUBLE:
                    {
                        datasize = 8;
                        fwrite(data[ip][ir].data(), datasize, ehdr.columns, fp);
                    }
                    break;
                }
            }
        }
        break;
    case BIP:
        for (size_t ir=0; ir<ehdr.rows; ++ir)
        {
            for (size_t ic=0; ic<ehdr.columns; ++ic)
            {
                switch (ehdr.datatype)
                {
                case DT_BYTE:
                    {
                        datasize = 1;
                        vector<uint8_t> dataout(ehdr.planes);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            dataout[ip] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.planes, fp);
                    }
                    break;
                case DT_INT:
                    {
                        datasize = 2;
                        vector<int16_t> dataout(ehdr.planes);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            dataout[ip] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.planes, fp);
                    }
                    break;
                case DT_U_INT:
                    {
                        datasize = 2;
                        vector<uint16_t> dataout(ehdr.planes);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            dataout[ip] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.planes, fp);
                    }
                    break;
                case DT_LONG:
                    {
                        datasize = 4;
                        vector<int32_t> dataout(ehdr.planes);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            dataout[ip] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.planes, fp);
                    }
                    break;
                case DT_U_LONG:
                    {
                        datasize = 4;
                        vector<uint32_t> dataout(ehdr.planes);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            dataout[ip] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.planes, fp);
                    }
                    break;
                case DT_FLOAT:
                    {
                        datasize = 4;
                        vector<float> dataout(ehdr.planes);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            dataout[ip] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.planes, fp);
                    }
                    break;
                case DT_DOUBLE:
                    {
                        datasize = 8;
                        vector<double> dataout(ehdr.planes);
                        for (size_t ip=0; ip<ehdr.planes; ++ip)
                        {
                            dataout[ip] = data[ip][ir][ic];
                        }
                        fwrite(dataout.data(), datasize, ehdr.planes, fp);
                    }
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    fclose(fp);
    return datasize * ehdr.planes * ehdr.columns * ehdr.rows;

}

int32_t write_envi_data(string name, size_t columns, size_t rows, size_t planes, uint8_t datatype, uint8_t interleave, uint8_t *data)
{
    envi_hdr ehdr;
    int32_t iretn = 0;

    ehdr.basename = name;
    ehdr.columns = columns;
    ehdr.rows = rows;
    ehdr.planes = planes;
    ehdr.datatype = datatype;
    ehdr.interleave = interleave;
    ehdr.offset = 0;
    if (BYTE_ORDER == LITTLE_ENDIAN)
    {
        ehdr.byteorder = BO_INTEL;
    }
    else
    {
        ehdr.byteorder = BO_NETWORK;
    }
    iretn = write_envi_data(ehdr, data);
    return iretn;

}

int32_t write_envi_data(envi_hdr &ehdr, uint8_t *data)
{
    int32_t iretn = 0;

    iretn = write_envi_hdr(ehdr);
    if (iretn < 0)
    {
        return iretn;
    }

    size_t extension;
    string fname;
    if ((extension = ehdr.basename.find_last_of(".")) != string::npos)
    {
        fname = ehdr.basename;
    }
    else {
        fname = ehdr.basename.substr(0, extension);
        switch (ehdr.interleave)
        {
        case BSQ:
            fname += ".bsq";
            break;
        case BIL:
            fname += ".bil";
            break;
        case BIP:
            fname += ".bip";
            break;
        default:
            fname += ".img";
            break;
        }
    }
    FILE *fp = fopen(fname.c_str(), "wb");
    if (fp == nullptr)
    {
        return -errno;
    }

    uint8_t datasize=1;
    switch (ehdr.datatype)
    {
    case DT_BYTE:
        datasize = 1;
        break;
    case DT_INT:
    case DT_U_INT:
        datasize = 2;
        break;
    case DT_LONG:
    case DT_U_LONG:
    case DT_FLOAT:
        datasize = 4;
        break;
    case DT_DOUBLE:
        datasize = 8;
        break;
    }

    switch (ehdr.interleave)
    {
    case BSQ:
        for (size_t ip=0; ip<ehdr.planes; ++ip)
        {
            for (size_t ir=0; ir<ehdr.rows; ++ir)
            {
                fwrite(data, datasize, ehdr.columns, fp);
            }
        }
        break;
    case BIL:
        for (size_t ir=0; ir<ehdr.rows; ++ir)
        {
            for (size_t ip=0; ip<ehdr.planes; ++ip)
            {
                fwrite(data, datasize, ehdr.columns, fp);
            }
        }
        break;
    case BIP:
        for (size_t ir=0; ir<ehdr.rows; ++ir)
        {
            for (size_t ip=0; ip<ehdr.columns; ++ip)
            {
                fwrite(data, datasize, ehdr.planes, fp);
            }
        }
        break;
    default:
        break;
    }

    fclose(fp);
    return datasize * ehdr.planes * ehdr.columns * ehdr.rows;
}
