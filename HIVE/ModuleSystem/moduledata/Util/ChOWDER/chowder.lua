ChOWDER = {}
setmetatable(ChOWDER, {__index = HiveBaseModule})

ChOWDER.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ChOWDER})
    this.id = "id_" .. string.byte(math.random(100000000))
    return this
end

function ChOWDER:Do()
    -- nothing to do
    self:UpdateValue()
    local v = self.value
    local image = v.image
    if (image == nil) then
        print "image nil"
        return false
    end
    
    local saver = ImageSaver()
    local imageBuffer = saver:SaveMemory(1, image)
    local imageBufferSize = saver:MemorySize()

    -- create metabinary
    local metabin = require("Network").MetaBinary()
    local json = [[{
            "jsonrpc" : "2.0",
            "method" : "AddContent",
            "to" : "master",
            "params" : {
                "id" : "]] .. self.id .. [[",
                "type" : "image",
                "width" : "512",
                "height" : "512"
            }
    }]]
    
    print(json)
    metabin:Create(json, imageBuffer, imageBufferSize) 

    print('Send:', v.url);
    -- send through websocket
    if (self.ws == nil) then
        self.ws = require("Network").Connection()
        self.ws:Connect(v.url) --'ws://localhost:8082/v1/')
    end
    self.ws:SendBinary(metabin:BinaryBuffer(), metabin:BinaryBufferSize())
    --ws:Close()
    
    return true
end
