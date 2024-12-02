import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";

// 1. 创建场景
const scene = new THREE.Scene();

// 2. 创建环境光（设置为白色，强度为 1）
const ambientLight = new THREE.AmbientLight(0xff000, 1);  // 参数：颜色和强度
scene.add(ambientLight);

// 3. 创建相机
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

// 4. 创建渲染器
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

let controls, stats;
controls = new OrbitControls(camera, renderer.domElement);
controls.update();


// 5. 创建一个物体（例如一个立方体）并添加到场景
const geometry = new THREE.BoxGeometry();
const material = new THREE.MeshStandardMaterial({ color: 0x0077ff });
const cube = new THREE.Mesh(geometry, material);
scene.add(cube);

// 6. 设置相机的位置
camera.position.z = 5;

// 7. 渲染函数
function animate() {
    requestAnimationFrame(animate);

    // 让物体转动
    cube.rotation.x += 0.01;
    cube.rotation.y += 0.01;

    renderer.render(scene, camera);
}

// 开始动画
animate();





