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
    print('vol=', vol)
    if vol == nil then
        return false
    end
    
    local w = vol:Width()
    local h = vol:Height()
    local d = vol:Depth()    
    local c = vol:Component()    
    
    if (w == 0 or h == 0 or d == 0) then
        print('Size zero=', w, h, d)
        return false
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
        "varname": "]] .. self.varname .. [["
        },
        "id":0
    }]]
        
    local imageBuffer = vol:Pointer()
    print('imagebuffer=', imageBuffer)
    local imageBufferSize = w * h * d * 4
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
    print('send!!!!!!!!!!!', imageBufferSize, self.varname);
    return true
end

function ParallelCoordinate:color()
    return {0,0,0,0}
end
