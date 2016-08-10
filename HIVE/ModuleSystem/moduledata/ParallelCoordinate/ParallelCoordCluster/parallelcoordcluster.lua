ParallelCoordCluster = {}
setmetatable(ParallelCoordCluster, {__index = HiveBaseModule})

ParallelCoordCluster.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ParallelCoordCluster})
    this.gentex = GenTexture()
    this.volumeclustering = require('ClusterParallelCoord').VolumeClustering()
    return this
end

function ParallelCoordCluster:Do()
    self:UpdateValue()
    self:PrintValue()

    if self.value.volume == nil then
        return 'No volume input'
    end

    print('Clustring = ', self.volumeclustering:Execute(self.value.volume))
    
    -- dump
    local axisNum = self.volumeclustering:GetAxisNum()
    local ax
    print('AxisNum = ' .. axisNum)
    for ax=0, axisNum-1 do
        local cnum = self.volumeclustering:GetClusterNum(ax)
        print('ClusterNum = ' .. cnum)
    
        for c=0,cnum-1 do
            local cv = self.volumeclustering:GetClusterValue(ax, c)
            for i,v in pairs(cv) do
                print(i,v)
            end
        end
    end

    print('End')

    return true
end

function ParallelCoordCluster:select()
    self:UpdateValue()
    return self.gentex:ImageData()
end
