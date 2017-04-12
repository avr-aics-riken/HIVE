
MandelbulbVolumeGenerator = {}
setmetatable(MandelbulbVolumeGenerator, {__index = HiveBaseModule})

MandelbulbVolumeGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = require("MandelbulbVolGen")()
    setmetatable(this, {__index=MandelbulbVolumeGenerator})
    return this
end

function MandelbulbVolumeGenerator:Do()
    self:UpdateValue()
    return self.gen:Generate(self.value.size)
end

function MandelbulbVolumeGenerator:VolumeData()
    local v = self.value
    return self.gen:VolumeData()
end
