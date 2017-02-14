ImageSet = {}
setmetatable(ImageSet, {__index = HiveBaseModule})

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
        local imageBuffer = self.saver:SaveMemory(5, image)
        local imageBufferSize = self.saver:MemorySize()
        self.loader:LoadMemory(5, imageBuffer, imageBufferSize);
        local copy = self.loader:ImageData();
        
        setBufferData(id, copy);
        print("image saved")
    end
    return true
end
