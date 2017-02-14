ImageSet = {}
setmetatable(ImageSet, {__index = HiveBaseModule})

ImageSet.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ImageSet})
    return this
end

function ImageSet:Do()
    self:UpdateValue()
    local v = self.value
    local id = v.id
    local image = v.image
    if image ~= nil and id ~= "" then
        local a = setBufferData(id, image);
        print("image saved")
    end
    return true
end
