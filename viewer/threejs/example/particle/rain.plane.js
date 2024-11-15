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
camera.position.set(0, 0, 100); // 调整相机位置
camera.lookAt(0, 0, 0);

const cubeGeometry = new THREE.BoxGeometry(1, 1, 1);
const cubeMaterial = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
const cube = new THREE.Mesh(cubeGeometry, cubeMaterial);
scene.add(cube);

// 创建渲染器
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// 环境光（Ambient Light）：提供均匀的全局光照
const ambientLight = new THREE.AmbientLight(0xffffff, 1); // 调整光照颜色和强度
scene.add(ambientLight);

// 定向光（Directional Light）：模拟来自一个方向的强光（类似太阳光）
const directionalLight = new THREE.DirectionalLight(0xffffff, 0.5);
directionalLight.position.set(0, 1, 0);
scene.add(directionalLight);

let controls, stats;

controls = new OrbitControls(camera, renderer.domElement);
controls.update();

stats = new Stats();
document.body.appendChild(stats.dom);

// 2. 创建雨滴的材质
const textureLoader = new THREE.TextureLoader();
const dropTexture = textureLoader.load("../textures/sprites/rain02.png");

const vertexShader = `
    uniform float time;
    attribute vec3 velocity;

    varying vec2 vUv;
    varying vec3 vp;

    void main() {
        vUv = uv;

        // 获取每个实例的变换矩阵并进行变换
        vec4 transformed = instanceMatrix * vec4(position, 1.0);

        // 添加动画
        transformed.y += velocity.y * time;

        vp = vec3(instanceMatrix[3][0], instanceMatrix[3][1], instanceMatrix[3][2]);

        // 对变换后的顶点做最终的模型视图变换
        gl_Position = projectionMatrix * modelViewMatrix * transformed;

        // 法线也需要进行变换
        // normal = normalize((instanceMatrix * vec4(normal, 0.0)).xyz);
    }
`;

// 4. 片段着色器
const fragmentShader = `
    uniform sampler2D rainTexture;
    varying vec2 vUv;
    varying vec3 vp;
    void main() {
        vec4 texColor = texture2D(rainTexture, vUv);
        texColor.a *= 0.7;
        gl_FragColor = texColor;
        gl_FragColor = vec4(vp, 1.0);
    }
`;

const dropMaterial = new THREE.ShaderMaterial({
    uniforms: {
        rainTexture: { value: dropTexture },
        time: { value: 0 },
    },
    vertexShader: vertexShader,
    fragmentShader: fragmentShader,
    transparent: true,
    depthWrite: false,
    side: THREE.DoubleSide,
});

// 3. 创建一个PlaneGeometry作为雨滴的基本几何体
const dropGeometry = new THREE.PlaneGeometry(0.021, 1); // 减小雨滴的尺寸

// 4. 创建一个InstancedMesh来批量渲染雨滴
const numDrops = 1000; // 增加雨滴数量
const rainArea = 50; // 定义雨区范围
const instancedMesh = new THREE.InstancedMesh(
    dropGeometry,
    dropMaterial,
    numDrops
);
scene.add(instancedMesh);

// 5. 为每个雨滴分配初始位置和速度
const dropsData = []; // 存储每个雨滴的初始数据（位置、速度）
const velocityArray = new Float32Array(numDrops * 3);

for (let i = 0; i < numDrops; i++) {
    // 在一个合理的范围内随机分布雨滴
    const x = Math.random() * rainArea - rainArea / 2; // -25 到 25
    const y = Math.random() * rainArea - rainArea / 2; // -25 到 25
    const z = Math.random() * rainArea - rainArea / 2; // -25 到 25

    const position = new THREE.Vector3(x, y, z);
    velocityArray[i * 3 + 0] = 0;
    velocityArray[i * 3 + 1] = -18.2;
    velocityArray[i * 3 + 2] = 0;

    dropsData.push({ position });

    // 为每个雨滴设置变换矩阵
    const matrix = new THREE.Matrix4();
    matrix.setPosition(position);
    instancedMesh.setMatrixAt(i, matrix);
}

const velocityAttribute = new THREE.BufferAttribute(velocityArray, 3);
instancedMesh.geometry.setAttribute("velocity", velocityAttribute);

instancedMesh.instanceMatrix.needsUpdate = true;

// 6. 设置相机位置
camera.position.z = 50;

// 7. 更新雨滴位置的函数
function updateDrops() {
    for (let i = 0; i < dropsData.length; i++) {
        const drop = dropsData[i];
        drop.position.add(drop.velocity);

        // 当雨滴落到底部时，重置到顶部
        if (drop.position.y < -rainArea / 2) {
            drop.position.y = rainArea / 2;
            drop.position.x = Math.random() * rainArea - rainArea / 2;
            drop.position.z = Math.random() * rainArea - rainArea / 2;
        }

        const matrix = new THREE.Matrix4();
        matrix.setPosition(drop.position);

        instancedMesh.setMatrixAt(i, matrix);
    }
    instancedMesh.instanceMatrix.needsUpdate = true;
}

console.log(instancedMesh);
// 动画渲染函数
function animate() {
    requestAnimationFrame(animate);

    // updateDrops(); // 更新粒子位置

    // cube.rotation.x += 0.1;
    // cube.rotation.y += 0.1;
    const times = dropMaterial.uniforms.time.value / 0.016
    if ( times >= 6) {
        dropMaterial.uniforms.time.value = 0.0;
    }
    dropMaterial.uniforms.time.value += 0.016;

    renderer.render(scene, camera);
    stats.update();
    // controls.update();
}

// 启动动画循环
animate();
