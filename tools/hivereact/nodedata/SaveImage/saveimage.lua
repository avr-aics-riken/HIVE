SaveImage = {}
setmetatable(SaveImage, {__index = BaseComponent})

SaveImage.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=SaveImage})
    this.saver = ImageSaver()
    return this
end

function SaveImage:Do()
    self:UpdateValue()
    local v = self.value
	self.saver:Save(v.filepath, v.image)
end

