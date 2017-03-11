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

function ImageSet:SendImageIDs()
    if targetClientId == nil then
        return
    end
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
end

function ImageSet:Do()
    self:UpdateValue()
    local v = self.value
    local id = v.id
    local image = v.image
    local selected = v.selected
    
    if image ~= nil and id ~= "" then
        -- Copy image
        self.loader:LoadImageData(image);
        local copy = self.loader:ImageData();
        
        setBufferData(id, copy);
        self:SendImageIDs();
        print("image saved")
    end
    
    if v.delete == true then
        selected = string.gsub(selected, "'", "\"")
        local selectList = JSON.decode(selected)
        if selectList ~= nil then
            if #selectList.selected > 0 then
                for i, image in ipairs(selectList.selected) do
                    deleteBufferData(image);
                    print("ImageSet:" .. image .. " deleted")
                end
            end
        end
        self:SendImageIDs();
    end
    if v.deleteall == true then
        clearBufferData();
        self:SendImageIDs();
        print("ImageSet: delete all the images")
    end
    
    return true
end
