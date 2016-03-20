SaveImage = {}
setmetatable(SaveImage, {__index = HiveBaseModule})

SaveImage.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=SaveImage})
    this.saver = ImageSaver()
    return this
end

function SaveImage:Do()
    self:UpdateValue()
    local v = self.value
	return self.saver:Save(v.filepath, v.image)
end

