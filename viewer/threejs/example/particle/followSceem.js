import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";

// 创建场景
const scene = new THREE.Scene();

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

const geometry = new THREE.BoxGeometry(1, 1, 1);
const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
const cube = new THREE.Mesh(geometry, material);
scene.add(cube);

// 创建渲染器
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// // 添加环境光
// const ambientLight = new THREE.AmbientLight(0xfff);
// scene.add(ambientLight);

// // 添加点光源
// const pointLight = new THREE.PointLight(0xffffff, 1, 100);
// pointLight.position.set(10, 10, 10);
// scene.add(pointLight);

let controls, stats;

controls = new OrbitControls(camera, renderer.domElement);
controls.target.set(0, 10, 0);
controls.minDistance = 25;
controls.maxDistance = 15000;
controls.maxPolarAngle = Math.PI / 1.7;
controls.autoRotate = false;
controls.autoRotateSpeed = -1;
controls.update();

stats = new Stats();
document.body.appendChild(stats.dom);

const particleCount = 100;
const particlesGeometry = new THREE.BufferGeometry();

const historyLength = 20; // 历史轨迹长度
const mouseHistory = new Array(historyLength).fill(new THREE.Vector3());
let isMoving = false;
let lastMouseMoveTime = 0;
const moveTimeout = 100; // 多少毫秒没有移动就认为停止了

const particlesMaterial = new THREE.ShaderMaterial({
    uniforms: {
        size: { value: 24.0 },
        mouseHistory: { value: mouseHistory },
        historyLength: { value: historyLength },
        time: { value: 0 },
        isMoving: { value: 0.0 }, // 添加移动状态uniform
    },
    vertexShader: `
        uniform float size;
        uniform vec3 mouseHistory[20];
        uniform int historyLength;
        uniform float time;
        uniform float isMoving;

        varying vec3 vColor;
        varying float vAlpha;

        void main() {
            float particleIndex = position.x * float(historyLength - 1);
            int index = int(particleIndex);

            vec3 pos1 = mouseHistory[index];
            vec3 pos2 = mouseHistory[min(index + 1, historyLength - 1)];
            float t = fract(particleIndex);
            vec3 pos = mix(pos1, pos2, t);

            // 计算到当前鼠标位置（第一个历史点）的距离
            float distToMouse = distance(pos, mouseHistory[0]);

            // 当停止移动时，使用距离来收缩轨迹
            float shrinkFactor = mix(
                exp(-distToMouse * 3.0), // 停止时的收缩效果
                1.0,                      // 移动时保持原样
                isMoving
            );

            // 应用收缩效果
            pos = mix(mouseHistory[0], pos, shrinkFactor);

            // 波动效果
            float wave = sin(time * 2.0 + particleIndex * 0.5) * 0.02 * shrinkFactor;
            pos.x += wave;
            pos.y += wave;

            // 渐变效果
            float fadeEffect = 1.0 - float(index) / float(historyLength - 1);
            fadeEffect *= shrinkFactor; // 应用收缩效果到透明度
            vAlpha = fadeEffect * 0.8;

            // 颜色渐变
            vColor = mix(
                vec3(1.0, 0.5, 0.0),
                vec3(0.0, 0.5, 1.0),
                fadeEffect
            );

            gl_Position = vec4(pos, 1.0);
            gl_PointSize = size * (0.3 + fadeEffect * 0.7);
        }
    `,
    fragmentShader: `
        varying vec3 vColor;
        varying float vAlpha;

        void main() {
            vec2 center = gl_PointCoord - vec2(0.5);
            float dist = length(center);

            // 创建柔和的粒子边缘
            float alpha = smoothstep(0.5, 0.2, dist) * vAlpha;

            gl_FragColor = vec4(1.0,0,0, alpha);
        }
    `,
    transparent: true,
    blending: THREE.AdditiveBlending,
    depthWrite: false,
});

// 初始化粒子位置，只需要x坐标在0到1之间即可
const particlesPosition = new Float32Array(particleCount * 3);
for (let i = 0; i < particleCount; i++) {
    particlesPosition[i * 3] = i / (particleCount - 1); // x 均匀分布在0-1之间
    particlesPosition[i * 3 + 1] = 0;
    particlesPosition[i * 3 + 2] = 0;
}

particlesGeometry.setAttribute(
    'position',
    new THREE.BufferAttribute(particlesPosition, 3)
);

// 创建粒子系统
const particles = new THREE.Points(particlesGeometry, particlesMaterial);
scene.add(particles);



// 鼠标位置变量
const mouse = new THREE.Vector3();
const mouseWorld = new THREE.Vector3();
const plane = new THREE.Plane(new THREE.Vector3(0, 0, 1), 0);

// 捕获鼠标移动事件
document.addEventListener("mousemove", onMouseMove, false);

function onMouseMove(event) {
    // 计算新的鼠标位置
    mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
    mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
    mouse.z = 0;

    // 只有当鼠标移动距离超过阈值时才更新历史记录
    const lastPos = mouseHistory[0];
    const dist = mouse.distanceTo(lastPos);
    if (dist > 0.01) { // 调整此值可以控制轨迹的平滑度
        mouseHistory.pop();
        mouseHistory.unshift(mouse.clone());
    }

    // 更新移动状态
    isMoving = true;
    lastMouseMoveTime = Date.now();
}

// 动画渲染函数
function animate() {
    requestAnimationFrame(animate);

    // 检查是否停止移动
    if (isMoving && Date.now() - lastMouseMoveTime > moveTimeout) {
        isMoving = false;
    }

    cube.rotation.x += 0.10;
    cube.rotation.y += 0.10;

    // 更新uniforms
    particles.material.uniforms.time.value += 0.01;
    particles.material.uniforms.mouseHistory.value = mouseHistory;
    particles.material.uniforms.isMoving.value = isMoving ? 1.0 : 0.0;

    console.log(camera)
    renderer.render(scene, camera);
    stats.update();
    controls.update();
}

// 启动动画循环
animate();
