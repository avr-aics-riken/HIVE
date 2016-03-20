ParallelCoordinate = {}
setmetatable(ParallelCoordinate, {__index = HiveBaseModule})

ParallelCoordinate.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ParallelCoordinate})
    this.volQ = require('VolumeQuantizer')()
    print('VVVVVVVVVVVV', this.volQ)
    return this
end


function sendData(varname, voldata, w, h, d, c, qbit, datasize)
    local minmaxstring = ''
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
        "minmax":[ ]] .. minmaxstring .. [[ ]
        },
        "id":0
    }]]
    --print(json)
    local imageBuffer = voldata:Pointer()
    print('ImageBuffer=', imageBuffer)
    local imageBufferSize = w * h * d * datasize * c
    print('imageBufferSize=', imageBufferSize)
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    local bbuffer = HIVE_metabin:BinaryBuffer()
    network:SendBinary(bbuffer, HIVE_metabin:BinaryBufferSize())
    print('send!!!!!!!!!!!', imageBufferSize, varname);
end

function ParallelCoordinate:Do()    
    self:UpdateValue()
    self:PrintValue()
    
    if self.value.coordinate == nil then
        return 'No coordiante input'
    end
    for i,v in pairs(self.value.coordinate) do -- dump
        print('coordinate:', i, v)
        for j,k in pairs(v) do
            print(j,k)
        end
    end
    
    local qbit = self.value.quantizeBit
    local sdiv = self.value.sampleingDiv
    local c = 0
    self.volQ:Clear()
    self.volQ:QuantizeSize(qbit)
    self.volQ:SamplingNum(sdiv[1], sdiv[2], sdiv[3])

    local voldim = {}           
    for i,v in pairs(self.value.coordinate) do
        print('coordinate:', i, v)
        if v.volume then
            c = c + 1
            if (self.value.userMinmax) then
                self.volQ:AddCustomMinMax(v.volume, v.min, v.max);
            else
                self.volQ:Add(v.volume)
            end
        end
    end
    
    if (c == 0) then
        return 'No coordinate input(invalid VolumeInput)'
    end
    
    local r = self.volQ:Create(vol)
    if r == false then
        return "Faild to create data."
    end
    print('CREATE=', r)
    
    voldata = self.volQ:VolumeData()
    
    --print('volsize =', voldata:GetNum() )
    --send
    sendData(self.varname, voldata, sdiv[1], sdiv[2], sdiv[3], c, qbit, 4)    
    
    if true then
        return true
    end
    ---------------------------------
--[[    
    local vol = self.value.volume
    local img = self.value.image
    print('vol=', vol, img)
    if vol == nil and img == nil then
        return "Invalid data"
    end
    local mode = 'raw'
    local qsize = 1

    print('volvolvolvolvolvolvolvolvolvolvolvolvol', self.oldvol, vol)
    if self.oldvol == vol then
        return true
    end
    self.oldvol = vol

    local minmaxstring = ''
    if vol then
        local volquant = require('VolumeQuantizer')()
        self.volquant = volquant
        volquant:Create(vol)
        local minmax = volquant:GetMinMax()
        for i,v in pairs(minmax) do
            if i == 1 then
                minmaxstring = minmaxstring .. '{'
            else
                minmaxstring = minmaxstring .. ',{'
            end
            local flg = false;
            for j,k in pairs(v) do
                if not(flg) then
                    minmaxstring = minmaxstring .. '"' .. j .. '":"' .. k .. '"'
                else
                    minmaxstring = minmaxstring .. ',"' .. j .. '":"' .. k .. '"'
                end
                flg = true
                print('VQ:MinMax', i,j,k)
            end
            minmaxstring = minmaxstring .. "}"
        end
        vol = volquant:VolumeData()
        mode = 'pack'
        qsize = 8
        self.vol = vol
    end

    local w
    local h
    local d = 1
    local c
    local dat
    local imageBuffer
    local datasize
    if vol then
        w = vol:Width()
        h = vol:Height()
        d = vol:Depth()
        c = vol:Component()
        if (w == 0 or h == 0 or d == 0) then
            print('Size zero=', w, h, d)
            return "Size zero:" .. w .. ',' .. h .. ',' .. d 
        end
        imageBuffer = vol:Pointer()
        datatype = 'float';
        datasize = 4
    else
        dat = img
        w = img:GetWidth()
        h = img:GetHeight()
        c = 4
        if (w == 0 or h == 0) then
            print('Size zero=', w, h)
            return "Size zero:" .. w .. ',' .. h .. ',' .. d 
        end
        imageBuffer = img:GetBuffer()
        datatype = 'byte';
        datasize = 1
    end
--]]    
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
--[[    print(json)
    print('imagebuffer=', imageBuffer)
    local imageBufferSize = w * h * d * datasize * c
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    self.bbuffer = HIVE_metabin:BinaryBuffer()
    network:SendBinary(self.bbuffer, HIVE_metabin:BinaryBufferSize())
    print('send!!!!!!!!!!!', imageBufferSize, self.varname);
    --]]
    return true
end

function ParallelCoordinate:color()
    return {0,0,0,0}
end
