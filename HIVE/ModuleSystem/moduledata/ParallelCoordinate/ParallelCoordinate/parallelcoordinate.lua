ParallelCoordinate = {}
setmetatable(ParallelCoordinate, {__index = HiveBaseModule})

ParallelCoordinate.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ParallelCoordinate})
    this.volQ = require('VolumeQuantizer')()
    this.volCache = {}
    --print('VVVVVVVVVVVV', this.volQ)
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
    
    if #self.volCache == #self.value.coordinate then
        local newvol = false
        for i,v in pairs(self.value.coordinate) do
            if findTable(self.volCache, v.volume) == false then
                newvol = true
            end 
        end
        if newvol == false then
            return true    
        end
    end
    
    self.volCache = {} -- clear
    for i,v in pairs(self.value.coordinate) do
        self.volCache[#self.volCache + 1] = v.volume
    end
    
        
    local qbit = self.value.quantizeBit
    local sdiv = self.value.sampleingDiv
    local c = 0
    self.volQ:Clear()
    self.volQ:QuantizeSize(qbit)
    self.volQ:SamplingNum(sdiv[1], sdiv[2], sdiv[3])

    local voldim = {}
    local label = {}
    for i,v in pairs(self.value.coordinate) do
        --print('coordinate:', i, v, v.userMinmax)
        if v.volume then
            c = c + 1
            if (v.userMinmax) then
                self.volQ:AddCustomMinMax(v.volume, v.min, v.max);
            else
                self.volQ:Add(v.volume)
            end
        end
        if v.label then
            table.insert(label, v.label)
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

    local minmax = self.volQ:GetMinMax()
    local minmaxstring = ''
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
    local labelstring = ''
    for i,v in pairs(label) do
        if i == 1 then
            labelstring = labelstring .. '"' .. v .. '"'
        else
            labelstring = labelstring .. ',"' .. v .. '"'
        end
    end
    
    voldata = self.volQ:VolumeData()
    local datasize = self.volQ:DataElementSize()
    
    --print('volsize =', voldata:GetNum() )
    --send
    sendData(self.varname, voldata, sdiv[1], sdiv[2], sdiv[3], c, qbit, datasize, minmaxstring, labelstring)
    
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

function ParallelCoordinate:select()
    self:UpdateValue()
    local sel = self.value.brushedIndex
    if sel == nil then
        return {}
    end
    
    local w, h, d    
    local v = self.value
    if #v.coordinate > 0 then
        local vol = v.coordinate[1].volume
        w = vol:Width()
        h = vol:Height()
        d = vol:Depth()
    else
        retrun {}    
    end
    
    local sdiv = self.value.sampleingDiv    
    local sw = w / sdiv[1]
    local sh = h / sdiv[2]
    local sd = d / sdiv[3]    
    local array = {}
    for i, v in pairs(sel) do
        local z = math.floor(v / (sdiv[1] * sdiv[2]))
        local y = math.floor((v - z*sdiv[1]*sdiv[2]) / sdiv[1])
        local x = (v % sdiv[1])
        --local x2 = v - (z*sdiv[1]*sdiv[2] + y*sdiv[1])
        --print(x,'=?',x2)
        array[#array + 1] = (x / sdiv[1] - 0.5) * w -- x*sw
        array[#array + 1] = (y / sdiv[2] - 0.5) * h -- y*sh
        array[#array + 1] = (z / sdiv[3] - 0.5) * d -- z*sd
    end 
    
    return array
end
