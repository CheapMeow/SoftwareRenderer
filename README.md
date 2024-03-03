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

获取世界空间中的三角形的法向方向，与视线方向点积，如果结果小于 0 则绘制