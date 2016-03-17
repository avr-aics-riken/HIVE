LoadImage = {}
setmetatable(LoadImage, {__index = BaseComponent})

LoadImage.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=LoadImage})
    this.loader = ImageLoader()
    return this
end

function LoadImage:Do()
    self:UpdateValue()
    local v = self.value
    self.loader:Load(v.filepath)
end

function LoadImage:image()
    local v = self.value
    return self.loader:ImageData()
end
