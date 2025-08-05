import * as THREE from "three";

import WebGL from "three/addons/capabilities/WebGL.js";

// 透视相机 PerspectiveCamera
// 视野角度 fov
// 变焦镜头 zoom
// 长宽比 aspect ratio
// 近裁面 near
// 远裁面 far
// 网格 mesh
// 材质 material
// 顶点 vertices
// 面 faces
// 渲染循环 render loop
// 动画循环 animate loop
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
);

const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

const geometry = new THREE.RingGeometry( 4.5, 5, 32 );
const material = new THREE.MeshBasicMaterial({
    color: 0xff0000,
});
// const geometry = new THREE.EdgesGeometry(geometry);
const cube = new THREE.Mesh(geometry, material);
scene.add(cube);

geometry.computeBoundingBox();
console.log(geometry)

camera.position.set(0, 0, 150); // 设置相机位置
camera.getObjectByProperty;

function animate() {
    requestAnimationFrame(animate);

    cube.rotation.x += 0.005;
    cube.rotation.y += 0.005;

    renderer.render(scene, camera);
}

if (WebGL.isWebGLAvailable()) {
    animate();
} else {
    const warning = WebGL.getWebGLErrorMessage();
    document.getElementById("container").appendChild(warning);
}
