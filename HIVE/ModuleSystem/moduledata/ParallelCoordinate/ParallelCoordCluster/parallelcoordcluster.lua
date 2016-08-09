ParallelCoordCluster = {}
setmetatable(ParallelCoordCluster, {__index = HiveBaseModule})

ParallelCoordCluster.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ParallelCoordCluster})
    this.gentex = GenTexture()
    return this
end

function sendData(varname, voldata, w, h, d, c, qbit, datasize, minmaxstring, label)
    local mode = 'raw'
    local datatype = 'float'
    local json = [[{
        "JSONRPC" : "2.0",
        "method" : "renderedImage",
        "to" : ]] .. targetClientId ..[[,
        "param" : {
        "type" : "volume",
        "width" : "]] .. w .. [[",
        "height" : "]] .. h .. [[",
        "depth" : "]] .. d .. [[",
        "component" : "]] .. c .. [[",
        "datatype": "]] .. datatype .. [[",
        "varname": "]] .. varname .. [[",
        "mode": "]] .. mode ..  [[",
        "quantsize": "]] .. qbit ..[[",
        "label":[ ]] .. label ..[[ ],
        "minmax":[ ]] .. minmaxstring .. [[ ]
        },
        "id":0
    }]]
    --print(json)
    local imageBuffer = voldata:Pointer()
    print('ImageBuffer=', imageBuffer)
    local imageBufferSize = w * h * d * datasize
    print('imageBufferSize=', imageBufferSize)
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    local bbuffer = HIVE_metabin:BinaryBuffer()
    network:SendBinary(bbuffer, HIVE_metabin:BinaryBufferSize())
    print('send!!!!!!!!!!!', imageBufferSize, varname);
end

function findTable(tbl, item)
    for key, value in pairs(tbl) do
        if value == item then return key end
    end
    return false
end

function ParallelCoordCluster:Do()
    self:UpdateValue()
    self:PrintValue()

    if self.value.volume == nil then
        return 'No volume input'
    end

    voldata = self.volQ:VolumeData()
    local datasize = self.volQ:DataElementSize()
    sendData(self.varname, voldata, sdiv[1], sdiv[2], sdiv[3], c, qbit, datasize, minmaxstring, labelstring)
    if true then
        return true
    end
    local json = [[{
        "JSONRPC" : "2.0",
        "method" : "renderedImage",
        "to" : ]] .. targetClientId ..[[,
        "param" : {
        "type" : "volume",
        "width" : "]] .. w .. [[",
        "height" : "]] .. h .. [[",
        "depth" : "]] .. d .. [[",
        "component" : "]] .. c .. [[",
        "datatype": "]] .. datatype .. [[",
        "varname": "]] .. self.varname .. [[",
        "mode": "]] .. mode ..  [[",
        "quantsize": "]] .. qsize ..[[",
        "minmax":[ ]] .. minmaxstring .. [[ ]
        },
        "id":0
    }]]
    return true
end

function ParallelCoordCluster:select()
    self:UpdateValue()
    return self.gentex:ImageData()
end
