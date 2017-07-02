
MandelbulbVolumeGenerator = {}
setmetatable(MandelbulbVolumeGenerator, {__index = HiveBaseModule})

MandelbulbVolumeGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = LoadModule("MandelbulbVolGen")
    setmetatable(this, {__index=MandelbulbVolumeGenerator})
    return this
end

function MandelbulbVolumeGenerator:Do()
    self:UpdateValue()
    local v = self.value
    return self.gen:GenerateOriginShift(v.size, v.originX, v.originY, v.originZ)
end

function MandelbulbVolumeGenerator:VolumeData()
    return self.gen:VolumeData()
end
