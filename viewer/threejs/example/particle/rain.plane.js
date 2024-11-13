import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";

// 创建场景
const scene = new THREE.Scene();

// 设置背景颜色为灰色
scene.background = new THREE.Color(0x808080);

// 创建相机
const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
);

// 设置相机位置
camera.position.z = 5;
// camera.position.z = -10;

const cubeGeometry = new THREE.BoxGeometry(1, 1, 1);
const cubeMaterial = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
const cube = new THREE.Mesh(cubeGeometry, cubeMaterial);
scene.add(cube);

// 创建渲染器
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// 环境光（Ambient Light）：提供均匀的全局光照
const ambientLight = new THREE.AmbientLight(0x00ff00, 1000);  // 灰色的环境光，强度为 1
scene.add(ambientLight);

// 定向光（Directional Light）：模拟来自一个方向的强光（类似太阳光）
const directionalLight = new THREE.DirectionalLight(0x00ff00, 1000); // 白色光源，强度为 1
directionalLight.position.set(0, 10, 10).normalize();  // 设置光源方向
scene.add(directionalLight);

let controls, stats;

controls = new OrbitControls(camera, renderer.domElement);
controls.update();

stats = new Stats();
document.body.appendChild(stats.dom);


// 2. 创建雨滴的材质
const textureLoader = new THREE.TextureLoader();
const dropTexture = textureLoader.load('../textures/sprites/rain02.png');

const dropMaterial = new THREE.MeshBasicMaterial({
    map: dropTexture,  // 使用加载的纹理
    opacity: 0.5,
    transparent: true,
    depthWrite: false,  // 防止雨滴遮挡其他雨滴
    side: THREE.DoubleSide,  // 双面渲染纹理，确保雨滴正反面都可见
    emissive: 0xffffff, // 让雨滴稍微发光，以便在低光条件下也可见
    roughness: 0.7,     // 设置材质的粗糙度，使得雨滴看起来更真实
    metalness: 0.2      // 设置金属度（调节不同材质效果）
});

// 3. 创建一个PlaneGeometry作为雨滴的基本几何体
const dropGeometry = new THREE.PlaneGeometry(0.025, 1);  // 每个雨滴是一个小平面

// 4. 创建一个InstancedMesh来批量渲染雨滴
const numDrops = 1000;  // 雨滴数量
const instancedMesh = new THREE.InstancedMesh(dropGeometry, dropMaterial, numDrops);
scene.add(instancedMesh);

instancedMesh.rotateZ(Math.PI / 12);

// 5. 为每个雨滴分配初始位置和速度
const dropsData = [];  // 存储每个雨滴的初始数据（位置、速度）

for (let i = 0; i < numDrops; i++) {
    // 随机分布每个雨滴的初始位置
    const y = Math.random() * 50 - 25;  // 随机重置Y坐标为上方
    const x = Math.random() * 50 - 25; // 随机X坐标
    const z = Math.random() * 50 - 25; // 随机X坐标
    const position = new THREE.Vector3(x, y, z);

    const velocity = new THREE.Vector3(0, -Math.random() * 0.5 - 0.05, 0);  // 随机下落速度

    dropsData.push({ position, velocity });

    // 为每个雨滴设置变换矩阵
    const matrix = new THREE.Matrix4();
    matrix.setPosition(position);
    instancedMesh.setMatrixAt(i, matrix);  // 将矩阵应用到每个实例
}

// 6. 设置相机位置
camera.position.z = 50;

// 7. 更新雨滴位置的函数
function updateDrops() {
    for (let i = 0; i < dropsData.length; i++) {
        const drop = dropsData[i];
        drop.position.add(drop.velocity);  // 更新雨滴位置

        // 如果雨滴落到地面（Y坐标小于-50），重置其位置
        if (drop.position.y < -10) {
            drop.position.y = Math.random() * 50 - 25;  // 随机重置Y坐标为上方
            drop.position.x = Math.random() * 50 - 25; // 随机X坐标
            drop.position.z = Math.random() * 50 - 25; // 随机X坐标
        }

        // 更新该雨滴的变换矩阵
        const matrix = new THREE.Matrix4();
        matrix.setPosition(drop.position);
        instancedMesh.setMatrixAt(i, matrix);  // 将新的矩阵应用到实例
    }

    instancedMesh.instanceMatrix.needsUpdate = true;  // 标记实例矩阵需要更新
}

// 动画渲染函数
function animate() {
    requestAnimationFrame(animate);

    updateDrops();  // 更新粒子位置

    // cube.rotation.x += 0.1;
    // cube.rotation.y += 0.1;

    renderer.render(scene, camera);
    stats.update();
    // controls.update();
}

// 启动动画循环
animate();
