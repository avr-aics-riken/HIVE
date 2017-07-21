SaveImage = {}
setmetatable(SaveImage, {__index = HiveBaseModule})

SaveImage.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=SaveImage})
    this.saver = ImageSaver()
    return this
end

function SaveImage:executeSave()
    local v = self.value
    if v.only_export then
        if HIVE_EXPORTMODE then
            return self.saver:Save(v.filepath, v.image)
        else
            return true
        end
    else
        return self.saver:Save(v.filepath, v.image)
    end
end

function SaveImage:Do()
    self:UpdateValue()
    
    if mpiMode() == true then
        if mpiRank() == 0 then
            return self:executeSave()
        end
    else
        return self:executeSave()
    end
end

