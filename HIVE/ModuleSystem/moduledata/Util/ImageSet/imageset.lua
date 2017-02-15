ImageSet = {}
setmetatable(ImageSet, {__index = HiveBaseModule})

local JSON = require('dkjson')

ImageSet.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ImageSet})
    this.saver = ImageSaver()
    this.loader = ImageLoader()
    return this
end

function ImageSet:Do()
    self:UpdateValue()
    local v = self.value
    local id = v.id
    local image = v.image
    if image ~= nil and id ~= "" then
        -- Copy image through png data
        local imageBuffer = self.saver:SaveMemory(5, image)
        local imageBufferSize = self.saver:MemorySize()
        self.loader:LoadMemory(5, imageBuffer, imageBufferSize);
        local copy = self.loader:ImageData();
        
        setBufferData(id, copy);
        local imageIDs = getBufferDataNames()
        
        if network then
            local data = {}
            data["JSONRPC"] = "2.0"
            data["method"] = "luaData"
            data["to"] = targetClientId
            data["id"] = 0
            data["param"] = {}
            data["param"]["name"] = "ImageSet"
            data["param"]["type"] = "imageids"
            data["param"]["images"] = imageIDs
            network:SendJSON( JSON.encode(data) );
        end
        print("image saved")
    end
    
    return true
end
