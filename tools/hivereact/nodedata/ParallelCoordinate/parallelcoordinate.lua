ParallelCoordinate = {}
setmetatable(ParallelCoordinate, {__index = BaseComponent})

ParallelCoordinate.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=ParallelCoordinate})
    return this
end

function ParallelCoordinate:Do()
    self:UpdateValue()

    local vol = self.value.volume
    local img = self.value.image
    print('vol=', vol, img)
    if vol == nil and img == nil then
        return false
    end
    local mode = 'raw'
    local qsize = 1

--[[
    local volquant = require('VolumeQuantizer')()
    self.volquant = volquant
    volquant:Create(vol)
    local minmax = volquant:GetMinMax()
    for i,v in pairs(minmax) do
        for j,k in pairs(v) do    
            print('VQ:MinMax', i,j,k)
        end
    end
    vol = volquant:VolumeData()
    mode = 'pack'
    qsize = 8
    self.vol = vol    
--]]
    
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
            return false
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
            return false
        end
        imageBuffer = img:GetBuffer()
        datatype = 'byte';
        datasize = 1
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
        "quantsize": "]] .. qsize ..[["
        },
        "id":0
    }]]
        
    print('imagebuffer=', imageBuffer)
    local imageBufferSize = w * h * d * datasize * c
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    self.bbuffer = HIVE_metabin:BinaryBuffer()
    network:SendBinary(self.bbuffer, HIVE_metabin:BinaryBufferSize())
    print('send!!!!!!!!!!!', imageBufferSize, self.varname);
    return true
end

function ParallelCoordinate:color()
    return {0,0,0,0}
end
