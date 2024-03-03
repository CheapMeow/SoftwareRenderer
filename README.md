# SoftwareRenderer

## 画线

Bresenham 画线算法

## 画三角形

给定三个点，获得缓冲的一个 bbox，遍历 bbox 内的每个像素点，三角形顶点到像素点的向量和三角形的边向量做叉乘

都是同一个方向，说明点在三角形内，那么就可以填充

这就就完成了在一个 bbox 内填充一个三角形

## 视口裁剪

三角形经过投影变换之后，三角形的部分可能在视口外

此时要裁剪掉看不到的部分。不能直接丢弃这个三角形，因为这个三角形本身也有要渲染的部分。

而是先传入这个三角形到片元着色阶段（函数），然后在片元着色里面，查看这个像素点的 UV 是不是超出屏幕了，超出则丢弃这个像素。

## 背面剔除

获取世界空间（经过视口变换之后）中的三角形的法向方向，与视线方向点积，如果结果小于 0 则绘制

## 视锥剔除

为每一个物体创建一个 boundingbox

经过投影变换之后，如果这个物体的 boundingbox 不在裁剪空间里面，就把整个物体丢掉

## 场景类

摄像机和物体的容器

执行物体层级的视锥体裁剪，来确定哪些物体将会被提交到 RenderManager 的队列

## 管理器

SceneManager 负责场景的加载和切换

RenderManager 负责处理渲染队列

SoftwareRenderer 负责渲染单位物体，执行屏幕裁剪，调用顶点着色，执行透视除法，调用 Rasterizer 渲染图元

+ 屏幕裁剪就是判断三角形是不是三个点都在 NDC 空间或者说裁剪空间之外，感觉也可以是称为三角形层面的视锥剔除

Rasterizer 负责渲染图元，进行深度测试，然后进行片元着色

## 片元着色器

假设三角形三个顶点为 A、B、C

重心坐标的计算是 AP x AB、BP x BC、CP x CA

所以第 0 个重心坐标对应第 2 个顶点的系数，第 1 个重心坐标对应第 0 个顶点的系数，第 2 个重心坐标对应第 1 个顶点的系数 